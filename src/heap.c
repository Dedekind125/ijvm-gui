#include <stdlib.h>
#include "ijvm.h"
#include "heap.h"

extern IJVM ijvm;

void init_heap()
{
    ijvm.heap.allocatedSize = 2;
    ijvm.heap.table = calloc((unsigned long)ijvm.heap.allocatedSize, sizeof(Array));
    ijvm.heap.size = 0;
}

int add_array_to_heap(int length)
{
    Array *newArray = malloc(sizeof(Array));
    newArray->length = length;
    newArray->items = malloc((unsigned long)length * sizeof(int32_t));
    
    if (ijvm.heap.size == ijvm.heap.allocatedSize){
        ijvm.heap.allocatedSize += 2;
        ijvm.heap.table = realloc(ijvm.heap.table,
                                 (unsigned long)ijvm.heap.allocatedSize * sizeof(Array));

        for (int i = ijvm.heap.size; i < ijvm.heap.allocatedSize; i++){
            ijvm.heap.table[i] = NULL;
        }
    }
    int arrayref = find_free_space();
    ijvm.heap.table[arrayref] = newArray;
    ijvm.heap.size += 1;

    return arrayref;
}

int find_free_space()
{
    for (int i = 0; i < ijvm.heap.allocatedSize; i++){
        if (ijvm.heap.table[i] == NULL){
            return i;
        }
    }
    printf("ERROR: Cannot find free space in the heap. HALT. \n");
    ijvm.isRunning = false;
    return -1;
}

void value_to_array(int arrayref, int index, int32_t value)
{
    int indexRange = (ijvm.heap.table[arrayref]->length)-1;
    
    if ( (index < 0) || (index > indexRange)){
        printf("ERROR: Trying to access invalid index \n");
        ijvm.isRunning = false;
    }
    ijvm.heap.table[arrayref]->items[index] = value;
}

int32_t get_array_value(int arrayref, int index)
{
    int indexRange = (ijvm.heap.table[arrayref]->length)-1;
    
    if ( (index < 0) || (index > indexRange)){
        printf("ERROR: Trying to access invalid index \n");
        ijvm.isRunning = false;
    }
    return ijvm.heap.table[arrayref]->items[index]; 
}

void destroy_heap()
{
    if (ijvm.heap.table == NULL) return;
    for (int i = 0; i < ijvm.heap.allocatedSize; i++){
        if (ijvm.heap.table[i] != NULL){
            free(ijvm.heap.table[i]->items);
            free(ijvm.heap.table[i]);
            ijvm.heap.table[i] = NULL;
        }
    }
    free(ijvm.heap.table);
    ijvm.heap.table = NULL;
}
