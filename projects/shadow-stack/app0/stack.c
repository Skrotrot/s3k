#include "stack.h"
#include "altc/altio.h"


Stack init_stack(void)
{
  Stack stack = {.stack_pointer = -1};
  return stack;
}

void push(Stack *stack, uint64_t cfi_ra)
{
  if (stack->stack_pointer >= (STACK_SIZE - 1)) {
    alt_printf("ERROR: SHADOW_STACK_OVERFLOW\n");
  }

  stack->stack_pointer++;
  stack->stack_frame[stack->stack_pointer] = cfi_ra;
}

uint64_t pop(Stack *stack)
{
  if (stack->stack_pointer < 0) {
    alt_printf("ERROR: SHADOW_STACK_UNDERFLOW\n");
    return 0;
  }

  uint64_t cfi_ra_now = stack->stack_frame[stack->stack_pointer];
  stack->stack_pointer--;
  return cfi_ra_now;
}

