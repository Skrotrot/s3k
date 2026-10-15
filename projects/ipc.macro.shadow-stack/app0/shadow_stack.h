#pragma once

#include "s3k/s3k.h"
#define SHADOW_STACK_SIZE 1024

typedef enum {
	SHADOW_STACK_OK = 0,
	SHADOW_STACK_MISMATCH,
	SHADOW_STACK_UNDERFLOW,
	SHADOW_STACK_OVERFLOW,
	SHADOW_STACK_INVALID_PID,
} shadow_stack_result_t;

// Initialize or reset all process stacks before using them.
void shadow_stack_init(void);

shadow_stack_result_t shadow_stack_push(s3k_pid_t pid, uint64_t ra);

shadow_stack_result_t shadow_stack_pop(s3k_pid_t pid, uint64_t ra_now);
