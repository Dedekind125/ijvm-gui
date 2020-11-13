#ifndef IJVM_H
#define IJVM_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stack.h"
#include "heap.h"
#include "frame.h"

#define MAGIC_NUMBER 0x1DEADFAD

#define OP_BIPUSH         ((uint8_t) 0x10)
#define OP_DUP            ((uint8_t) 0x59)
#define OP_ERR            ((uint8_t) 0xFE)
#define OP_GOTO           ((uint8_t) 0xA7)
#define OP_HALT           ((uint8_t) 0xFF)
#define OP_IADD           ((uint8_t) 0x60)
#define OP_IAND           ((uint8_t) 0x7E)
#define OP_IFEQ           ((uint8_t) 0x99)
#define OP_IFLT           ((uint8_t) 0x9B)
#define OP_ICMPEQ         ((uint8_t) 0x9F)
#define OP_IINC           ((uint8_t) 0x84)
#define OP_ILOAD          ((uint8_t) 0x15)
#define OP_IN             ((uint8_t) 0xFC)
#define OP_INVOKEVIRTUAL  ((uint8_t) 0xB6)
#define OP_IOR            ((uint8_t) 0xB0)
#define OP_IRETURN        ((uint8_t) 0xAC)
#define OP_ISTORE         ((uint8_t) 0x36)
#define OP_ISUB           ((uint8_t) 0x64)
#define OP_LDC_W          ((uint8_t) 0x13)
#define OP_NOP            ((uint8_t) 0x00)
#define OP_OUT            ((uint8_t) 0xFD)
#define OP_POP            ((uint8_t) 0x57)
#define OP_SWAP           ((uint8_t) 0x5F)
#define OP_WIDE           ((uint8_t) 0xC4)

typedef struct IJVM
{
  uint8_t    *buffer;
  uint8_t    *constantsPool;
  uint8_t    *textData;

  Stack     stack;
  Frame     *currentFrame;
  Heap      heap;

  int       magicNumber;
  int       poolLength;
  int       textDataLength;
  int       pc;

  bool      isRunning;
} IJVM;

/**
 * Sets the pointers for the constant pool and the text data blocks 
 * and calculates their sizes.
 **/
void set_binary_blocks(void);

/**
 * Returns the word at the top of the stack of the current frame
 **/
int32_t top_element(void);

/**
 * Perform one instruction and return.
 * Returns true if an instruction was executed. Returns false if machine has
 * halted or encountered an error.
 **/
bool step(void);

/**
 * Run the vm with the current state until the machine halts.
 **/
void run(void);


/**
 * Sets the output of the IJVM instance to the provided file.
 **/
void set_output(FILE *f);


/**
 * Sets the input of the IJVM instance to the provided file.
 **/
void set_input(FILE *f);


/**
 * Initializes the IJVM with the binary file found at the provided argument.
 *
 * Returns  0 on success
 *         -1 on failure
 **/
int init_ijvm(char *binary_path);


/**
 * Free all memory allocated.
 */
void destroy_ijvm(void);

#endif
