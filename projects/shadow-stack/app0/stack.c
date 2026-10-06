#include "stack.h"
#include "altc/altio.h"

#define STACK_SIZE 1024

typedef struct Stack 
{
  uint64_t stack_frame[STACK_SIZE];
  int stack_pointer;
} Stack;


Stack* init_stack(void)
{

  static Stack stack = {.stack_pointer = -1};

  return &stack;
}

void action(void)
{
  alt_printf("Simulated action\n");
}

void push(Stack *stack, uint64_t cfi_ra)
{
  if (stack->stack_pointer < 1024) {
    stack->stack_pointer++;
    stack->stack_frame[stack->stack_pointer] = cfi_ra;
    }
  else {
    alt_printf("ERROR: SHADOW_STACK_OVERFLOW\n");
    //action();
  }
}

void pop(Stack *stack, uint64_t cfi_ra_now)
{
  if (stack->stack_pointer >= 0) {
    if (stack->stack_frame[stack->stack_pointer] != cfi_ra_now) {
      action();
    }
    stack->stack_pointer--;
  }
  else {
    alt_printf("ERROR: SHADOW_STACK_UNDERFLOW\n");
    //action();
  }
}

