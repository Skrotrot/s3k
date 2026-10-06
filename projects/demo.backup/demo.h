#pragma once

#include "altc/altio.h"
#include "s3k/s3k.h"

static int counter;

void run_backup_demo(void)
{
    alt_printf("\n");

    counter = 1;
    alt_printf("counter = %d\n", counter);

    alt_printf("s3k_backup_write() -> %d\n", s3k_backup_write());

    counter = 42;
    alt_printf("counter mutated to %d\n", counter);

    alt_printf("s3k_backup_read() -> %d\n", s3k_backup_read());

    alt_printf("recover declined; counter is still %d\n", counter);
}
