#include "altc/altio.h"
#include "s3k/s3k.h"

#include "../../tutorial-commons/utils.h"

int main(void)
{
	// Setup UART access
	setup_uart();

    // Value to increase
    uint64_t value = 10;

	alt_printf("Value before inc: %d\n", value);
    
    // Given the pointer too a uint64_t increases the value of the variable
    s3k_inc_val(&value);

	alt_printf("Value after inc: %d\n", value);

    for (int i = 0; i < 10; i++) {
        s3k_inc_val(&value);
    }
    
    alt_printf("Value after some more inc: %d\n", value);
}
