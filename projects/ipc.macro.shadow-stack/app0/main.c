/*
MONITOR DEMO

Responsible for answering IPC from user-created app. 

*/ 

#include "altc/altio.h"
#include "s3k/s3k.h"
#include "altc/string.h"
#include "../../tutorial-commons/utils.h"
#include "stack.h"
void local_scheduling()
{
	s3k_cap_delete(HART2_TIME);
	s3k_cap_delete(HART3_TIME);

	s3k_cap_delete(HART1_TIME);
	uint32_t cap_idx = find_free_cap();
	log_sys("Time derivation", 
		s3k_cap_derive(HART0_TIME, cap_idx, s3k_mk_time(S3K_MIN_HART, 0, S3K_SLOT_CNT / 4)));
	log_sys("Time delegation", s3k_mon_cap_move(MONITOR, APP0_PID, cap_idx, APP1_PID, APP_1_TIME));

}

int terminate_process(int monitor, int app1_id, s3k_state_t state)
{
	
		// s3k_cap_revoke(HART0_TIME);
	s3k_mon_suspend(MONITOR, APP1_PID);
	
	if (s3k_mon_state_get(MONITOR, APP1_PID, &state) == S3K_SUCCESS)
	{
		alt_printf("app state: 0x%x\n", state);
	}
	return 0; 
}

int main(void)
{
	// Setup UART access
	setup_uart();
	alt_printf("DEMO: Starting IPC monitor\n");

	// Setup app1 capabilities and PC
	setup_app_1();

	// Setup scheduling
	local_scheduling();
	
	uint32_t socket = setup_socket(true, false, false);
	s3k_mon_resume(MONITOR, APP1_PID);
	
	uint32_t free_cap_idx = find_free_cap();
	s3k_msg_t reply = {0};
	s3k_reply_t receive;
	s3k_state_t state;
	s3k_err_t err;
	uint64_t latest = {0};
	uint64_t push_value[4] = {0};
	memcpy(reply.data, "1", 1);
	int adress_stored = 0;
	
	Stack *shadow_stack = init_stack();
	//debg 
	int running = 1;
	while (running) {
		receive = s3k_sock_recv(socket, free_cap_idx);
		alt_printf("	FROM APP1: 0x%x \n",  receive.data[0]);
		
		if(receive.data[1])
		{
			push(shadow_stack, receive.data[0]);
			s3k_sock_send(socket, &reply);
		}
		else
		{
			uint64_t address_from_stack = pop(shadow_stack);
			alt_printf("	FROM STACK: 0x%x \n", address_from_stack);
			if((memcmp(&receive.data[0], &address_from_stack, sizeof(receive.data))))
			{
				alt_printf("Allowed!\n");
				s3k_sock_send(socket, &reply);
			}
			else
			{
				alt_printf("Denied!\n");
				s3k_sock_send(socket, &reply);
				running = terminate_process(MONITOR, APP1_PID, state);
			}
		}
	}
}

