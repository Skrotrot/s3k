#pragma once

#include "proc.h"

extern int procs_reset_count[S3K_PROC_CNT];

#ifndef S3K_PROC_MAX_RESETS
#define S3K_PROC_MAX_RESETS 3
#endif

#define S3K_PROC_MEM_SIZE 0x10000

typedef struct {
	uint64_t regs[REG_CNT];
	uint8_t mem[S3K_PROC_MEM_SIZE];
} backup_t;

extern backup_t _backup[S3K_PROC_CNT];
extern char _backup_end[];

void process_backup_init(void);

bool process_backup_write(pid_t pid);
bool process_backup_recover(pid_t pid);
