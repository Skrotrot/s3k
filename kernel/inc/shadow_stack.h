#pragma once

#include "cap_types.h"

typedef enum {
	SHADOW_STACK_OK = 0,
	SHADOW_STACK_MISMATCH,
	SHADOW_STACK_UNDERFLOW,
	SHADOW_STACK_OVERFLOW,
} shadow_stack_result_t;

void shadow_stack_init(void);

shadow_stack_result_t shadow_stack_push(pid_t pid, uint64_t ra);

shadow_stack_result_t shadow_stack_pop(pid_t pid, uint64_t ra);
