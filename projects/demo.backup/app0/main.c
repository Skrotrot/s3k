#include "altc/altio.h"
#include "s3k/s3k.h"

#include "../../tutorial-commons/utils.h"
#include "../demo.h"

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
    run_backup_demo();

    do {} while(1);
}
