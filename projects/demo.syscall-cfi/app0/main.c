#include "altc/altio.h"
#include "s3k/s3k.h"

#include "../../tutorial-commons/utils.h"
#include "../demo.h"
#include "s3k/syscall.h"

#define TRAP_STACK_SIZE 1024

char trap_stack[TRAP_STACK_SIZE];

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

    setup_cfi_trap(trap_stack, TRAP_STACK_SIZE);

    s3k_backup_write();

    run_shadow_stack_demo();

    do {} while(1);
}
