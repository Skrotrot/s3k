#include "altc/altio.h"
#include "altc/string.h"
#include "s3k/s3k.h"
#include <stdint.h>
#include "../app_sections.h"
#include "../../tutorial-commons/utils.h"

#define APP_0_BAK_PMP_SLOT 3
#define APP_0_PMP_SLOT 4
#define LOCAL_APP_0_BAK_BASE_ADDR 0x80020000
#define LOCAL_APP_1_BASE_ADDR 0x80030000
#define LOCAL_APP_1_SIZE 0x10000

static uint32_t free_cap_idx;
static uint32_t free_cap_bak_mem_idx;
static uint32_t free_cap_bak_idx;
static uint32_t free_cap_pmp_idx;
static uint32_t free_reset_cap_index;

static uint64_t app1_addr;
static uint64_t uart_addr;

char trap_stack[1024];

void trap_handler(void) __attribute__((interrupt("machine")));
void access_backup_mem();
void move_pmp_access(uint32_t free_cap_idx);
void unload_move_pmp();
void reset_process();
void memory_copy(uint32_t dst_address, uint32_t src_address, uint32_t size);
void print_out_memory(unsigned int base_address, int size);
int terminate_process(int monitor, int app1_id, s3k_state_t state);


/* 
	Function gives app0 access to a region in which the program will create the backup of app1.
	This function creates a memory region and loads pmp access to app0. 
*/
void access_backup_mem(){
	uint64_t app0_bak_addr = s3k_napot_encode(LOCAL_APP_0_BAK_BASE_ADDR, LOCAL_APP_1_SIZE);
	uint32_t free_cap_bak_mem_idx = find_free_cap();
	log_sys("app0 backup memory: region created                    |", s3k_cap_derive(RAM_MEM, free_cap_bak_mem_idx, s3k_mk_memory(LOCAL_APP_0_BAK_BASE_ADDR, LOCAL_APP_0_BAK_BASE_ADDR + LOCAL_APP_1_SIZE, S3K_MEM_RWX)));
	uint32_t free_cap_bak_idx = find_free_cap();
	log_sys("app0 backup memory: pmp cap derived                   |", s3k_cap_derive(free_cap_bak_mem_idx, free_cap_bak_idx, s3k_mk_pmp(app0_bak_addr, S3K_MEM_RWX)));

	log_sys("app0 backup memory: region loaded to app0 for access. |", s3k_pmp_load(free_cap_bak_idx, APP_0_BAK_PMP_SLOT));

	s3k_sync_mem();
}

/* 
	Function gives app0 access to memory dedicated to app1.
	This function creates a memory region and loads pmp access to app0. 
*/	
void access_app1_mem(){

	app1_addr = s3k_napot_encode(LOCAL_APP_1_BASE_ADDR, LOCAL_APP_1_SIZE);
	uint32_t free_cap_mem_idx = find_free_cap();
	log_sys("app1 memory: region created                           |", s3k_cap_derive(RAM_MEM, free_cap_mem_idx, s3k_mk_memory(LOCAL_APP_1_BASE_ADDR, LOCAL_APP_1_BASE_ADDR + LOCAL_APP_1_SIZE, S3K_MEM_RWX)));
	free_cap_idx = find_free_cap();
	log_sys("app1 memory: pmp cap derived                          |", s3k_cap_derive(free_cap_mem_idx, free_cap_idx, s3k_mk_pmp(app1_addr, S3K_MEM_RWX)));

	log_sys("app1 memory: pmp region loaded to app0 for access.    |", s3k_pmp_load(free_cap_idx, APP_0_PMP_SLOT));

	s3k_sync_mem();
}

/* 
	Function moves app1 dedicated memory pmp access from app0 to app1.
	This function unloads pmp access from app0, move capability to app1 and loads pmp.
*/
void move_pmp_access(uint32_t free_cap_idx)
{
	
	s3k_sync_mem();
	log_sys("app1 memory: pmp region unloaded from app0.           |", s3k_pmp_unload(free_cap_idx));
	
	log_sys("app1 memory: region moved app0 -> app1                |", s3k_mon_cap_move(MONITOR, APP0_PID, free_cap_idx, APP1_PID, APP_1_CAP_PMP_MEM));

	log_sys("app1 memory: pmp region loaded to app1 for access.    |", s3k_mon_pmp_load(MONITOR, APP1_PID, APP_1_CAP_PMP_MEM, APP_1_PMP_SLOT_MEM));
}

/* 
	Function is used in the reset stage when app1 needs to be reset by app0.
	This function moves pmp access from app0 to app1, loads new uart capability and resets the program counter. 
*/
void unload_move_pmp()
{
	move_pmp_access(free_cap_idx);

	uart_addr = s3k_napot_encode(UART0_BASE_ADDR, 0x8);
	// Derive a PMP capability for uart
	log_sys("uart: derive |", s3k_cap_derive(UART_MEM, free_cap_idx, s3k_mk_pmp(uart_addr, S3K_MEM_RW)));
	log_sys("uart; move   |", s3k_mon_cap_move(MONITOR, APP0_PID, free_cap_idx, APP1_PID, APP_1_CAP_PMP_UART));
	log_sys("uart: load   |", s3k_mon_pmp_load(MONITOR, APP1_PID, APP_1_CAP_PMP_UART, APP_1_PMP_SLOT_UART));

	// Write start PC of app1 to PC
	log_sys("7",
		s3k_mon_reg_write(MONITOR, APP1_PID, S3K_REG_PC, LOCAL_APP_1_BASE_ADDR));

	s3k_sync_mem();
}

/* 
	Function is used in the reset stage when memory from backup region is copied back to app1 memory region.
	This function suspends the process, unloads pmp from app1, moves capability to app0, loads pmp, copy memory and move back pmp access to app1. 
*/
void reset_process()
{

	free_reset_cap_index = find_free_cap();
	s3k_mon_suspend(MONITOR, APP1_PID);


	log_sys("app1 memory: pmp region unloaded from app1.           |", s3k_mon_pmp_unload(MONITOR, APP1_PID, APP_1_CAP_PMP_MEM));

	log_sys("app1 memory: region moved app1 -> app0                |", s3k_mon_cap_move(MONITOR, APP1_PID, APP_1_CAP_PMP_MEM, APP0_PID, free_reset_cap_index));

	log_sys("app1 memory: pmp region loaded to app0 for access.    |", s3k_pmp_load(free_reset_cap_index, APP_0_PMP_SLOT));

	s3k_sync_mem();
	memory_copy(LOCAL_APP_1_BASE_ADDR, LOCAL_APP_0_BAK_BASE_ADDR, LOCAL_APP_1_SIZE);

	log_sys("reset program counter                                 |", s3k_mon_reg_write(MONITOR, APP1_PID, S3K_REG_PC, LOCAL_APP_1_BASE_ADDR));
	log_sys("reset stack pointer                                   |", s3k_mon_reg_write(MONITOR, APP1_PID, S3K_REG_SP, app1_stack_pointer));

	move_pmp_access(free_reset_cap_index);

	s3k_sync_mem();

}

/* 
	Function copy memory of specified size from src to dst address.
*/
void memory_copy(uint32_t dst_address, uint32_t src_address, uint32_t size){
	volatile uint32_t *dst_memory = (volatile uint32_t *)(uintptr_t)dst_address;
	volatile uint32_t *src_memory = (volatile uint32_t *)(uintptr_t)src_address;
	
	s3k_sleep(1000000);
	memcpy((void *)dst_memory, (void *)src_memory, size);
	alt_printf("\nmemory region copied\n");

}

/* 
	Function prints out memory as a debug function.
*/
void print_out_memory(unsigned int base_address, int size)
{
	volatile uint32_t *memory = (volatile uint32_t *)(uintptr_t)base_address;
	for (int i = 0; i < size; i++) 
	{
		uint32_t value = memory[i];
		alt_printf("0x%x: 0x%x\n", base_address + i * 4, value);
	}
}

/* 
	Function suspends the process and prints out the state of the closed process.
*/
int terminate_process(int monitor, int app1_id, s3k_state_t state)
{
	s3k_mon_suspend(MONITOR, APP1_PID);
	if (s3k_mon_state_get(MONITOR, APP1_PID, &state) == S3K_SUCCESS)
	{
		alt_printf("app state: 0x%x\n", state);
	}
	return 0; 
}

void local_scheduling()
{
	s3k_cap_delete(HART2_TIME);
	s3k_cap_delete(HART3_TIME);

	s3k_cap_delete(HART1_TIME);
	uint32_t cap_idx = find_free_cap();
	log_sys("Time derivation", 
		s3k_cap_derive(HART0_TIME, cap_idx, s3k_mk_time(S3K_MIN_HART, 0, S3K_SLOT_CNT / 2)));
	log_sys("Time delegation", s3k_mon_cap_move(MONITOR, APP0_PID, cap_idx, APP1_PID, APP_1_TIME));

}

void trap_handler(void)
{
	default_trap_handler();
}

int main(void)
{

	uint32_t free_cap_idx = find_free_cap();
	s3k_msg_t reply = {0};
	s3k_reply_t receive;
	s3k_state_t state;
	s3k_err_t err;
	uint64_t latest[4] = {0};

	// Setup UART access
	setup_uart();
	setup_trap(trap_handler, trap_stack, 1024);	
	alt_printf("DEMO: Starting IPC monitor\n");
	
	alt_printf("backup size : 0x%x\n", (app1_stack_pointer - app1_data));
	access_backup_mem();
	access_app1_mem();
	//print_out_memory(0x80030000, 1700);
	//copy data + bss + stack 
	memory_copy(LOCAL_APP_0_BAK_BASE_ADDR, LOCAL_APP_1_BASE_ADDR, LOCAL_APP_1_SIZE);

	unload_move_pmp();
	//local_scheduling();
	setup_scheduling(ROUND_ROBIN);
	uint32_t socket = setup_socket(true, false, false);
	s3k_mon_resume(MONITOR, APP1_PID);


	reply.data[1] = 1;
	int adress_stored = 0;

	int running = 1;
	int reset_threshold = 0;

	while (running) {
		receive = s3k_sock_recv(socket, free_cap_idx);
		alt_printf("FROM APP1: \"%s\" \n", ((char *)receive.data));

		if (!adress_stored){
			memcpy(latest, receive.data, sizeof(receive.data));
			s3k_sock_send(socket, &reply);

			adress_stored = 1;
		}
		else if((memcmp(receive.data, latest, sizeof(latest))))
		{
			alt_printf("Allowed!\n");
			s3k_sock_send(socket, &reply);
			memset(latest, 0, 1);

			adress_stored = 0;
		}
		else
		{
			adress_stored = 0;
			if (reset_threshold == 4){
				running = terminate_process(MONITOR, APP1_PID, state);
				break;
			}
			alt_printf("Denied! Resetting process\n");
			s3k_sock_send(socket, &reply);


			//s3k_sleep(10000000);
			reset_process();
			s3k_mon_resume(MONITOR, APP1_PID);

			reset_threshold += 1;
		}
	}

	//s3k_state_t state;
	//terminate_process(MONITOR, APP1_PID, state);
}
