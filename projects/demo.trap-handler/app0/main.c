#include "altc/altio.h"
#include "s3k/s3k.h"

#include "../../tutorial-commons/utils.h"
#include "../demo.h"
#include "s3k/syscall.h"

#define TRAP_STACK_SIZE 1024

char trap_stack[TRAP_STACK_SIZE];
void trap_handler(void) __attribute__((interrupt("machine")));

void debug_trap_trigger(void) __attribute__((interrupt("machine")));
void debug_trap_trigger(void) {
	uint64_t epc = s3k_reg_read(S3K_REG_EPC);
	uint64_t esp = s3k_reg_read(S3K_REG_ESP);
	uint64_t ecause = s3k_reg_read(S3K_REG_ECAUSE);
	uint64_t eval = s3k_reg_read(S3K_REG_EVAL);

	alt_printf(
			   "error info:\n- epc: 0x%x\n- esp: 0x%x\n- ecause: 0x%x\n- eval: 0x%x\n",
			   epc, esp, ecause, eval);
	alt_printf("restoring pc and sp\n\n");
    s3k_backup_read();
} 

void trap_handler(void)
{
	alt_puts(" === TRAP HANDLER TRIGGERED ===");
	debug_trap_trigger();
}

static char source[32];

void vulnerable(void)
{
    CFI_ENTER();
    char buffer[2];

    alt_printf("Inside vulnerable...\n");

    memset(source, 'A', 32); // Offset to RA

    *(uint64_t *)&source[32] = (uint64_t)win; // Overwrite RA with win() (Will succeed)

    memcpy(buffer, source, sizeof(source) + 64);

    CFI_RETURN();
}

int non_vulnerable(void)
{
    CFI_ENTER();
    char buffer[128];

    alt_printf("Inside non vulnerable...\n");

    memset(source, 'A', 32); // Offset to RA

    *(uint64_t *)&source[32] = (uint64_t)win; // Try to overwrite RA with win() (Will fail)

    memcpy(buffer, source, sizeof(source) + 64);

    CFI_RETURN(1);
}

int main(void)
{
    // Setup UART access
    setup_uart();

    setup_trap(trap_handler, trap_stack, TRAP_STACK_SIZE);

    s3k_backup_write();

    run_shadow_stack_demo();

    do {} while(1);
}
