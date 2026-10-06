#include "altc/altio.h"
#include "s3k/s3k.h"

#include "../../tutorial-commons/utils.h"
#include "stack.h"


void test_overflow(void)
{
  alt_printf("Epected:- ERROR: SHADOW_STACK_OVERFLOW\nActual:- ");
  Stack *shadow_stack = init_stack();

  for (uint64_t i = 0; i < 1026; i++) {
    push(shadow_stack, i);
  }

  alt_printf("\n\n");
}

void test_underflow(void)
{
  alt_printf("Epected:- ERROR: SHADOW_STACK_UNDERFLOW\nActual:- ");
  Stack *shadow_stack = init_stack();

  uint64_t i = 0;
  pop(shadow_stack, i);

  alt_printf("\n\n");
}

void test_matching(void)
{
  alt_printf("Epected:- \nActual:- ");
  Stack *shadow_stack = init_stack();

  for (uint64_t i = 1; i <= 100; i++) {
    push(shadow_stack, i);
  }

  for (uint64_t i = 100; i >= 1; i--) {
    pop(shadow_stack, i);
  }
  
  alt_printf("\n\n");
}

void test_conflict(void)
{
  alt_printf("Epected:- Simulated action\nActual:- ");
  Stack *shadow_stack = init_stack();

  uint64_t i = 0;
  uint64_t j = 1;

  push(shadow_stack, i);
  pop(shadow_stack, j);

  alt_printf("\n\n");
}

int main(void)
{
	// Setup UART access
	setup_uart();

  test_conflict();
  test_underflow();
  test_matching();
  test_overflow();
}
