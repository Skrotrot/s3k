#pragma once

#include "altc/altio.h"
#include "altc/string.h"
#include "s3k/s3k.h"

#define CFI_ENTER() \
    void *__cfi_ra; \
    __asm__ volatile ("mv %0, ra" : "=r"(__cfi_ra)); \
    s3k_shadow_stack_push(__cfi_ra); \
    alt_printf("Enter 0x%x\n", __cfi_ra)

#define CFI_RETURN(...) \
    do { \
        void *__cfi_ra_now; \
        __asm__ volatile ("ld %0, -8(s0)" : "=r"(__cfi_ra_now)); \
        bool __cfi_ok = s3k_shadow_stack_pop(__cfi_ra_now); \
        alt_printf("Exit 0x%x [%s]\n", __cfi_ra_now, \
                   __cfi_ok ? "OK" : "SHADOW STACK MISMATCH"); \
        return __VA_ARGS__; \
    } while (0)

// Defined in each of the programs to not have the same memory space
int non_vulnerable(void);
void vulnerable(void);

void win()
{
    alt_printf("Attack Works\n");
    do {} while(1);
}

void run_shadow_stack_demo(void)
{
    alt_printf("Before Non-Vulnerable\n");
    non_vulnerable();

    alt_printf("\n");

    alt_printf("Before Vulnerable\n");
    vulnerable();

    alt_printf("Successful exit\n");
}
