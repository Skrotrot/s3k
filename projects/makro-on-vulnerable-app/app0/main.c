#include "altc/altio.h"
#include "altc/string.h"
#include "s3k/s3k.h"

#include "../../tutorial-commons/utils.h"

static int _call_depth = 0;

#define CFI_ENTER() \
    void *__cfi_ra; \
    __asm__ volatile ("mv %0, ra" : "=r"(__cfi_ra)); \
    alt_printf("Enter %d   0x%x -\n", ++_call_depth, __cfi_ra)

#define CFI_RETURN(...) \
    void *__cfi_ra_now; \
    __asm__ volatile ("ld %0, 40(sp)" : "=r"(__cfi_ra_now)); \
    alt_printf("Exit  %d              - 0x%x\n", \
               _call_depth--, __cfi_ra_now); \
    return __VA_ARGS__; \

void win()
{
    alt_printf("Attack Works\n");
    do {} while(1);
}

static char source[32];

void vulnerable(void)
{
    CFI_ENTER();
    char buffer[2];

    alt_printf("Inside\n");

    memset(source, 'A', 32); //Offset to RA

    *(uint64_t *)&source[32] = (uint64_t)win; // Overwrite RA with win()

    memcpy(buffer, source, sizeof(source) + 64);
    CFI_RETURN();
}

int main(void)
{
    // Setup UART access
    setup_uart();

    alt_printf("Before\n");

    vulnerable();

    alt_printf("Successful exit\n");
}
