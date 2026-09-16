#include "altc/altio.h"
#include "s3k/s3k.h"

#include "../../tutorial-commons/utils.h"
#include "s3k/syscall.h"

int main(void)
{
	// Setup UART access
	setup_uart();

    alt_printf("Demo counter allocated and set to 0 in the kernel (per process)\n");

    alt_printf("Demo counter initially: %d\n", s3k_get_demo_val());

    s3k_inc_demo_val(1);
    alt_printf("Demo counter after first inc: %d\n", s3k_get_demo_val());

    s3k_inc_demo_val(5);
    alt_printf("Demo counter after more inc: %d\n", s3k_get_demo_val());

    for (int i = 0; i < 10; i++) {
        s3k_inc_demo_val(2);
    }
    alt_printf("Demo counter after even more increases: %d\n", s3k_get_demo_val());
}
