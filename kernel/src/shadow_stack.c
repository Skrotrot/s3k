#include "shadow_stack.h"
#include <stdint.h>

#ifndef S3K_SHADOW_STACK_SIZE
#define S3K_SHADOW_STACK_SIZE 1024
#endif

typedef struct {
	uint64_t frame[S3K_SHADOW_STACK_SIZE];
	int sp;
} shadow_stack_t;

static shadow_stack_t stacks[S3K_PROC_CNT];

void shadow_stack_init(void)
{
	for (uint64_t i = 0; i < S3K_PROC_CNT; i++)
		stacks[i].sp = -1;
}

shadow_stack_result_t shadow_stack_push(pid_t pid, uint64_t ra)
{
	shadow_stack_t *stack = &stacks[pid];

	if (stack->sp >= S3K_SHADOW_STACK_SIZE - 1)
		return SHADOW_STACK_OVERFLOW;

	stack->sp++;
	stack->frame[stack->sp] = ra;
	return SHADOW_STACK_OK;
}

shadow_stack_result_t shadow_stack_pop(pid_t pid, uint64_t ra)
{
	shadow_stack_t *stack = &stacks[pid];

	if (stack->sp < 0)
		return SHADOW_STACK_UNDERFLOW;
	bool match = stack->frame[stack->sp] == ra;

	stack->sp--;
	return match ? SHADOW_STACK_OK : SHADOW_STACK_MISMATCH;
}
