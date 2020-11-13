#ifndef STACK_H
#define STACK_H 

#include "ijvm.h"

typedef struct Stack
{
    int32_t  *items;
    int     allocatedSize;
    int     size;
} Stack;

/**
 * Initializes the stack by allocating an array with size 16.
 * and sets the (int)size of the stack to 0
 **/
void init_stack(void);

/**
 * Pushes a word into the stack.
 * @param item The value to be pushed into the stack
 **/
void stack_push(int32_t item);

/**
 *  Pops the top word from the stack.
 *  @returns The popped word from the stack.
 **/
int32_t stack_pop(void);

#endif
