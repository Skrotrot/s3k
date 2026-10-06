#include "s3k/s3k.h"

#ifndef STACK_H
#define STACK_H

typedef struct Stack Stack;

Stack* init_stack(void);

void push(Stack *stack, uint64_t cfi_ra);

uint64_t pop(Stack *stack);

#endif

