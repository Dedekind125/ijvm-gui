#include <assert.h>
#include <stdlib.h>
#include "ijvm.h"

extern IJVM ijvm;

void init_stack()
{
    ijvm.stack.allocatedSize = 16;
    ijvm.stack.items = malloc(sizeof(int32_t) * (unsigned long)ijvm.stack.allocatedSize);
    ijvm.stack.size = 0;
}

int32_t stack_pop(){
    assert(ijvm.stack.size > 0);
    return ijvm.stack.items[--ijvm.stack.size];
}

void stack_push(int32_t item){
    if(ijvm.stack.size == ijvm.stack.allocatedSize){
        ijvm.stack.allocatedSize *= 2;
        ijvm.stack.items = realloc(ijvm.stack.items,
                                  (unsigned long)ijvm.stack.allocatedSize * sizeof(int32_t));
    }
    ijvm.stack.items[ijvm.stack.size] = item;
    ijvm.stack.size += 1;
}

int32_t top_element(){
    assert(ijvm.stack.size > 0);
    return ijvm.stack.items[ijvm.stack.size-1];
}
