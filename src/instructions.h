#include "ijvm.h"

/**
 * This file contains the definitions of the IJVM instruction set functions. 
 * Each function executes one specific instruction and increments 
 * the program counter (pc) to point to the next instruction.
 **/

/**
 * Executes one instruction according to the opcode.
 * @returns 1 on success,
 *          0 on failere
 **/
bool execute_instruction(uint8_t opcode);

/**
 * Pushes a byte onto the stack; 
 **/
void bipush(void);

/**
 * Duplicates the top word of the stack.
 **/
void dup(void);

/**
 * Pops two words from stack and push their sum.
 **/
void iadd(void);

/**
 * Pop two words from stack; subtract the top word from 
 * the second to top word and push the result.
 **/
void isub(void);

/**
 * Pops two words from stack and push bit-wise AND.
 **/
void iand(void);

/**
 * Pops two words from stack and push bit-wise OR
 **/
void ior(void);

/**
 * Swap the two top words on the stack.
 **/
void swap(void);

/**
 * Delete word from top of stack.
 **/
void pop(void);

/**
 * Prefix instruction, next instruction has a 16-bit index;
 * calls mentioned instruction.
 **/
void wide(void);

/**
 * Pops a word from stack and store in local variable. 
 **/
void istore(bool);

/**
 * Push a local variable onto stack.
 **/
void iload(bool);

/**
 * Adds a constant value to a local variable.
 **/
void iinc(bool);

/**
 * Unconditional jump.
 **/
void _goto(void);

/**
 * Pops a word from stack and branch if it is zero.
 **/
void ifeq(void);

/**
 * Pops a word from stack and branch if it is less than zero; 
 **/
void iflt(void);

/**
 * Pops two words from stack and branch if they are equal.
 **/
void ifcmp(void);

/**
 * Push a constant from constant pool onto stack.
 **/
void ldcw(void);

/**
 * Invokes a method. It creates a new frame and pops the arguments from stack.
 * The pc is adjusted to point to the start of the method.
 **/
void invoke_virtual(void);

/**
 * Return from method. It restores the stack pointer; pushes the return word
 * to the stack and does frame managment.
 **/
void ireturn(void);

/**
 * Creates new array on the heap. Pops the array size from the stack
 * and pushes the arrayref back to the stack.
 **/
void newarray(void);

/**
 * Pushed the value stored in the array referenced by arrayref in the
 * location of the provided index. The arrayref and index are poped from the stack.
 **/
void iaload(void);

/**
 * Stores value at the index location in the array reference by arrayref.
 * The value, index and arrayref are poped from the stack.
 **/ 
void iastore(void);

/**
 * Pops port of the stack and start listening for network connection on the specific
 * port. Pushes 0 on the stack on failure, or the netref on success.
 **/
void netbind(void);

/**
 * Creates a network connection the speciefied host, port. 
 * Pushes 0 on the stack on failure, or the netref on success.
 **/
void netconnect(void);

/**
 * Pops netref from the stack and reads a character from the
 * network connection referenced by the netref.
 **/
void netin(void);

/**
 * Pops netref and a word from the stack and writes that character
 * to the network connection referenced by netref.
 **/
void netout(void);

/**
 * Pops netref from the stack and closes the network connection
 * reference by netref.
 **/
void netclose(void);

/**
 * Reads a character from the input and pushes it onto the stack. 
 * If no character is available, 0 is pushed.
 **/
void in(void);

/**
 * Pop word off stack and print it to the output file.
 **/
void out(void);
