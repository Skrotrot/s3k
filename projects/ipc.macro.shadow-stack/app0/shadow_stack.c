#include "shadow_stack.h"

typedef struct {
	uint64_t frame[SHADOW_STACK_SIZE];
	int sp;
} shadow_stack_t;

static shadow_stack_t shadow_stacks[S3K_PROC_CNT];

void shadow_stack_init(void)
{
	for (uint32_t pid = 0; pid < S3K_PROC_CNT; pid++)
		shadow_stacks[pid].sp = -1;
}

shadow_stack_result_t shadow_stack_push(s3k_pid_t pid, uint64_t ra)
{
	if (pid >= S3K_PROC_CNT)
		return SHADOW_STACK_INVALID_PID;

	shadow_stack_t *stack = &shadow_stacks[pid];
	if (stack->sp >= SHADOW_STACK_SIZE - 1)
		return SHADOW_STACK_OVERFLOW;

	stack->sp++;
	stack->frame[stack->sp] = ra;
	return SHADOW_STACK_OK;
}

shadow_stack_result_t shadow_stack_pop(s3k_pid_t pid, uint64_t ra)
{
	if (pid >= S3K_PROC_CNT)
		return SHADOW_STACK_INVALID_PID;

	shadow_stack_t *stack = &shadow_stacks[pid];
	if (stack->sp < 0)
		return SHADOW_STACK_UNDERFLOW;
	bool match = stack->frame[stack->sp] == ra;

	stack->sp--;
	return match ? SHADOW_STACK_OK : SHADOW_STACK_MISMATCH;
}
