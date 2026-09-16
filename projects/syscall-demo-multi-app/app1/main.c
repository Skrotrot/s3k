#include "altc/altio.h"
#include "s3k/s3k.h"

#include "../../tutorial-commons/utils.h"

int main(void)
{
	s3k_cap_revoke(APP_1_TIME);

    alt_printf("APP1 - Demo value: %d\n", s3k_get_demo_val());

    s3k_inc_demo_val(3);
    alt_printf("APP1 - Demo value: %d\n", s3k_get_demo_val());

    s3k_inc_demo_val(3);
    alt_printf("APP1 - Demo value: %d\n", s3k_get_demo_val());

    s3k_inc_demo_val(3);
    alt_printf("APP1 - Demo value: %d\n", s3k_get_demo_val());

    s3k_inc_demo_val(3);
    alt_printf("APP1 - Demo value: %d\n", s3k_get_demo_val());
}
