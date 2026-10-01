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

    memset(source, 'A', 40); // Offset to RA

    *(uint64_t *)&source[40] = (uint64_t)win; // Overwrite RA with win() (Will succeed)

    memcpy(buffer, source, sizeof(source) + 64);

    CFI_RETURN();
}

int main(void)
{
    alt_printf("\n[APP1]\n");
    run_shadow_stack_demo();

    do {} while(1);
}
