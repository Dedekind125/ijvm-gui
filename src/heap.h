#ifndef HEAP_H
#define HEAP_H

#include "ijvm.h"

#define OP_NEWARRAY       ((uint8_t) 0xD1)
#define OP_IALOAD         ((uint8_t) 0xD2)
#define OP_IASTORE        ((uint8_t) 0xD3)

typedef struct Array
{
    int32_t  *items;
    int      length;
} Array;

typedef struct Heap
{
    Array   **table;
    int     allocatedSize;
    int     size;
} Heap;

/**
 * Initializes the Heap with allocated size 2.
 **/
void init_heap(void);

/**
 * Adds and array to the heap.
 * @param length The length of the array
 **/
int add_array_to_heap(int length);

/**
 * Finds a free space where an array can be added to heap
 * @returns The index of the heap table where the array can be added, or
 *          -1 if it fails to find free space.
 **/
int find_free_space(void);

/**
 * Insert a value to an array in the heap
 * @param value the value to be inserted
 * @param arrayref The index of the array in the heap table
 * @param index The index where the value will be stored in the array.
 **/
void value_to_array(int value, int arrayref, int index);

/**
 * @param arrayref The index of the array in the heap table
 * @param index The index where the value is located in the array
 * @returns The value of table[arrayref][index]
 **/
int32_t get_array_value(int arrayref, int index);

/**
 * Free all memory allocations that where allocated by the Heap.
 **/
void destroy_heap(void);

#endif
