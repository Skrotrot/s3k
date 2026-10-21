#pragma once

#include "altc/altio.h"
#include "altc/string.h"
#include "s3k/s3k.h"

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
