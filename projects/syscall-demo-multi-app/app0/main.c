#include "altc/altio.h"
#include "s3k/s3k.h"

#include "../../tutorial-commons/utils.h"
#include "s3k/syscall.h"

int main(void)
{
	// Setup UART access
	setup_uart();

	// Setup app1 capabilities and PC
	setup_app_1();

	// Setup scehduling
	setup_scheduling(ROUND_ROBIN);

	// Start app1
	log_sys("10", s3k_mon_resume(MONITOR, APP1_PID)); 

    alt_printf("APP0 - Demo value: %d\n", s3k_get_demo_val());

    s3k_inc_demo_val(1);
    alt_printf("APP0 - Demo value: %d\n", s3k_get_demo_val());

    s3k_inc_demo_val(1);
    alt_printf("APP0 - Demo value: %d\n", s3k_get_demo_val());

    s3k_inc_demo_val(1);
    alt_printf("APP0 - Demo value: %d\n", s3k_get_demo_val());

    s3k_inc_demo_val(1);
    alt_printf("APP0 - Demo value: %d\n", s3k_get_demo_val());
}
