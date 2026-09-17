#include "s3k/s3k.h"

#ifndef STACK_H
#define STACK_H
#define STACK_SIZE 1024

typedef struct Stack 
{
  uint64_t stack_frame[STACK_SIZE];
  int stack_pointer;
} Stack;


Stack init_stack(void);

void push(Stack *stack, uint64_t cfi_ra);

uint64_t pop(Stack *stack);

#endif

