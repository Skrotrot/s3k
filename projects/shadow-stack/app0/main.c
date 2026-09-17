#include "altc/altio.h"
#include "s3k/s3k.h"

#include "../../tutorial-commons/utils.h"
#include "stack.h"


void test_overflow(void)
{
  alt_printf("Epected:- ERROR: SHADOW_STACK_OVERFLOW\nActual:- ");
  Stack shadow_stack = init_stack();

  for (uint64_t i = 0; i <= STACK_SIZE; i++) {
    push(&shadow_stack, i);
  }

  alt_printf("\n\n");
}

void test_underflow(void)
{
  alt_printf("Epected:- ERROR: SHADOW_STACK_UNDERFLOW\nActual:- ");
  Stack shadow_stack = init_stack();

  uint64_t i = 0;
  pop(&shadow_stack);

  alt_printf("\n\n");
}

void test_count(void)
{
  alt_printf("Epected:- 10--1 \nActual:- \n");
  Stack shadow_stack = init_stack();

  for (uint64_t i = 1; i <= 10; i++) {
    push(&shadow_stack, i);
  }

  for (uint64_t i = 1; i <= 10; i++) {
    alt_printf("%D\n", (unsigned long) pop(&shadow_stack));
  }
  
  alt_printf("\n\n");
}

int main(void)
{
	// Setup UART access
	setup_uart();

  test_underflow();
  test_count();
  test_overflow();
}
