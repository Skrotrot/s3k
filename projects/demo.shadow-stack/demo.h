#pragma once

#include "altc/altio.h"
#include "altc/string.h"
#include "s3k/s3k.h"

char *shadow_stack_result_str(s3k_shadow_stack_result_t result)
{
    switch (result) {
    case S3K_SHADOW_STACK_OK:
        return "SHADOW STACK OK";
    case S3K_SHADOW_STACK_MISMATCH:
        return "SHADOW STACK MISMATCH";
    case S3K_SHADOW_STACK_UNDERFLOW:
        return "SHADOW STACK UNDERFLOW";
    case S3K_SHADOW_STACK_OVERFLOW:
        return "SHADOW STACK OVERFLOW";
    }
    return "UNKNOWN";
}

#define CFI_ENTER() \
    void *__cfi_ra; \
    __asm__ volatile ("mv %0, ra" : "=r"(__cfi_ra)); \
    s3k_shadow_stack_result_t __cfi_push_result = s3k_shadow_stack_push(__cfi_ra); \
    alt_printf("Enter 0x%x [%s]\n", __cfi_ra, shadow_stack_result_str(__cfi_push_result))

#define CFI_RETURN(...) \
    do { \
        void *__cfi_ra_now; \
        __asm__ volatile ("ld %0, -8(s0)" : "=r"(__cfi_ra_now)); \
        s3k_shadow_stack_result_t __cfi_result = s3k_shadow_stack_pop(__cfi_ra_now); \
        alt_printf("Exit 0x%x [%s]\n", __cfi_ra_now, \
                   shadow_stack_result_str(__cfi_result)); \
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

void overflow(void)
{
    void *ra;
    __asm__ volatile ("mv %0, ra" : "=r"(ra));

    s3k_shadow_stack_result_t result;
    uint64_t pushes = 0;
    do {
        result = s3k_shadow_stack_push(ra);
        pushes++;
    } while (result == S3K_SHADOW_STACK_OK);

    alt_printf("Pushed %D times before [%s]\n", pushes,
	       shadow_stack_result_str(result));

    // Undo the successful pushes so later demos start with an empty stack.
    for (uint64_t i = 0; i < pushes - 1; i++)
        s3k_shadow_stack_pop(ra);
}

void run_shadow_stack_demo(void)
{
    alt_printf("Before Overflow\n");
    overflow();

    alt_printf("\n");

    alt_printf("Before Non-Vulnerable\n");
    non_vulnerable();

    alt_printf("\n");

    alt_printf("Before Vulnerable\n");
    vulnerable();

    alt_printf("Successful exit\n");
}
