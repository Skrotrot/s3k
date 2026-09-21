#include "altc/altio.h"
#include "s3k/s3k.h"

#include "../../tutorial-commons/utils.h"
#include "../demo.h"

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

    // Setup app1 capabilities and PC
    setup_app_1();

    setup_scheduling(ROUND_ROBIN);

    // Start app1
    s3k_mon_resume(MONITOR, APP1_PID);

    alt_printf("\n[APP0]\n");
    run_shadow_stack_demo();

    do {} while(1);
}
