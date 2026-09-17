#include "shadow_stack.h"

#define SHADOW_STACK_SIZE 1024

typedef struct {
	uint64_t frame[SHADOW_STACK_SIZE];
	int sp;
} shadow_stack_t;

static shadow_stack_t stacks[S3K_PROC_CNT];

void shadow_stack_init(void)
{
	for (uint64_t i = 0; i < S3K_PROC_CNT; i++)
		stacks[i].sp = -1;
}

bool shadow_stack_push(pid_t pid, uint64_t ra)
{
	shadow_stack_t *stack = &stacks[pid];

	if (stack->sp >= SHADOW_STACK_SIZE - 1)
		return false;

	stack->sp++;
	stack->frame[stack->sp] = ra;
	return true;
}

bool shadow_stack_pop(pid_t pid, uint64_t ra)
{
	shadow_stack_t *stack = &stacks[pid];

	if (stack->sp < 0)
		return false;
	bool match = stack->frame[stack->sp] == ra;

	stack->sp--;
	return match;
}
