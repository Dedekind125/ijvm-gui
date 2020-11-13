#include <stdlib.h>
#include <limits.h>
#include "ijvm.h"
#include "instructions.h"
#include "helpers.h"
#include "heap.h"
#include "network.h"

extern IJVM ijvm;
extern FILE *output;
extern FILE *input;

bool execute_instruction(uint8_t opcode){
    switch (opcode)
    {
    case OP_BIPUSH:
        bipush();
        break;
    
    case OP_DUP:
        dup();
        break;
    
    case OP_IADD:
        iadd();
        break;
    
    case OP_ISUB:
        isub();
        break;
    
    case OP_IAND:
        iand();
        break;
    
    case OP_IOR:
        ior();
        break;
    
    case OP_SWAP:
        swap();
        break;
    
    case OP_POP:
        pop();
        break;

    case OP_WIDE:
        wide();
        break;
    
    case OP_ISTORE:
        istore(false);
        break;
    
    case OP_ILOAD:
        iload(false);
        break;
    
    case OP_IINC:
        iinc(false);
        break;

	case OP_GOTO:
		_goto();
		break;
    
    case OP_IFEQ:
        ifeq();
        break;
    
    case OP_IFLT:
        iflt();
        break;
    
    case OP_ICMPEQ:
        ifcmp();
        break;

    case OP_LDC_W:
        ldcw();
        break;
    
    case OP_INVOKEVIRTUAL:
        invoke_virtual();
        break;
    
    case OP_IRETURN:
        ireturn();
        break;
    
    case OP_NEWARRAY:
        newarray();
        break;
    
    case OP_IALOAD:
        iaload();
        break;
    
    case OP_IASTORE:
        iastore();
        break;

    case OP_NETBIND:
        netbind();
        break;
    
    case OP_NETCONNECT:
        netconnect();
        break;
    
    case OP_NETIN:
        netin();
        break;
    
    case OP_NETOUT:
        netout();
        break;
    
    case OP_NETCLOSE:
        netclose();
        break;

    case OP_IN:
        in();
        break;

    case OP_OUT:
        out();
        break;
    
    case OP_NOP:
        ijvm.pc += 1;
        break;

    case OP_ERR:
        printf("Instruction ERR (0xFE) encountered! Halting...\n");
        return false;

    case OP_HALT:
        return false;

    default:
        return false;
    }
    return true;
}

void bipush()
{
    int8_t val = (int8_t) ijvm.textData[ijvm.pc+1];
    stack_push(val);
    
    ijvm.pc += 2;
}

void dup()
{
    stack_push(top_element());
    ijvm.pc += 1;
}

void iadd()
{
    int32_t val1 = stack_pop();
    int32_t val2 = stack_pop();
    
    stack_push(val1+val2);
    
    ijvm.pc += 1;
}

void isub()
{
    int32_t val1 = stack_pop();
    int32_t val2 = stack_pop();
    stack_push(val2-val1);
    
    ijvm.pc += 1;
}

void iand()
{
    int32_t val1 = stack_pop();
    int32_t val2 = stack_pop();
    stack_push(val2 & val1);
    
    ijvm.pc += 1;
}

void ior()
{
    int32_t val1 = stack_pop();
    int32_t val2 = stack_pop();
    stack_push(val2 | val1);
    
    ijvm.pc += 1;
}

void swap()
{
    int32_t val1 = stack_pop();
    int32_t val2 = stack_pop();

    stack_push(val1);
    stack_push(val2);
    
    ijvm.pc += 1;
}

void pop()
{
    stack_pop();
    ijvm.pc += 1;
}

void wide()
{
    uint8_t opcode = ijvm.textData[ijvm.pc+1];
    switch (opcode)
    {
        case OP_ISTORE:
            istore(true);
            break;

        case OP_ILOAD:
            iload(true);
            break;

        case OP_IINC:
            iinc(true);
            break;
    }
    ijvm.pc += 4;
}

void istore(bool wide)
{
    int32_t val = stack_pop();
    
    if(wide){
        uint16_t index = bytes_to_ushort(&ijvm.textData[ijvm.pc+2]);
        ijvm.currentFrame->items[index] = val;
    } else {
        uint8_t index = ijvm.textData[ijvm.pc+1];
        ijvm.currentFrame->items[index] = val;
        
        ijvm.pc += 2;
    }
}

void iload(bool wide)
{
    if(wide){
        uint16_t index = bytes_to_ushort(&ijvm.textData[ijvm.pc+2]);
        stack_push(ijvm.currentFrame->items[index]);
    } else {
        uint8_t index = ijvm.textData[ijvm.pc+1];
        stack_push(ijvm.currentFrame->items[index]);
        
        ijvm.pc += 2;
    }
}

void iinc(bool wide)
{
    if (wide){
        uint16_t index = bytes_to_ushort(&ijvm.textData[ijvm.pc+2]);
        int8_t addconst = (int8_t) ijvm.textData[ijvm.pc+3];
        ijvm.currentFrame->items[index] += addconst;
    } else {
        uint8_t index = ijvm.textData[ijvm.pc+1];
        int8_t addconst = (int8_t) ijvm.textData[ijvm.pc+2];
        ijvm.currentFrame->items[index] += addconst;

        ijvm.pc += 3;
    }
}

void _goto()
{
	int16_t offset = bytes_to_short(&ijvm.textData[ijvm.pc+1]);
	ijvm.pc += offset;
}

void ifeq()
{
    if (stack_pop() == 0){
        int16_t offset = bytes_to_short(&ijvm.textData[ijvm.pc+1]);
        ijvm.pc += offset;
    } 
    else {
        ijvm.pc += 3;
    }
}

void iflt()
{
    if (stack_pop() < 0){
        int16_t offset = bytes_to_short(&ijvm.textData[ijvm.pc+1]);
        ijvm.pc += offset;
    } 
    else {
        ijvm.pc += 3;
    }
}

void ifcmp()
{
    int32_t val1 = stack_pop();
    int32_t val2 = stack_pop();
    
    if (val1 == val2){
        int16_t offset = bytes_to_short(&ijvm.textData[ijvm.pc+1]);
        ijvm.pc += offset;
    } 
    else {
        ijvm.pc += 3;
    }
}

void ldcw()
{
    uint16_t index = bytes_to_ushort(&ijvm.textData[ijvm.pc+1]);
    int32_t constant = bytes_to_int(ijvm.constantsPool,index*4);
    stack_push(constant);

    ijvm.pc += 3;
}

void invoke_virtual()
{
    /* Calculate method offset in the text data */
    uint16_t index = bytes_to_ushort(&ijvm.textData[ijvm.pc+1]);
    int32_t methodOffset = bytes_to_int(ijvm.constantsPool,index*4);

    /* Calculate the number of args and the frame size of the method */
    uint16_t argsNum = bytes_to_ushort(&ijvm.textData[methodOffset]);
    uint16_t frameSize = bytes_to_ushort(&ijvm.textData[methodOffset+2]) + argsNum;

    Frame *methodFrame = new_frame(frameSize,argsNum);

    /* adjust the pc and save the old value */
    methodFrame->oldpc = ijvm.pc;
    methodFrame->oldsp = ijvm.stack.size;
    ijvm.pc = methodOffset+4;

    /* change the currentFrame pointer & save the previous Frame pointer */
    methodFrame->prev = ijvm.currentFrame;
    ijvm.currentFrame = methodFrame;
}

void ireturn()
{
    int32_t returnValue = stack_pop();
    ijvm.stack.size = ijvm.currentFrame->oldsp;
    stack_push(returnValue);

    Frame *oldFrame = ijvm.currentFrame;

    ijvm.pc = ijvm.currentFrame->oldpc+3;
    ijvm.currentFrame = ijvm.currentFrame->prev;

    free_frame(oldFrame);
}

void newarray(void)
{
    int32_t count = stack_pop();
    int arrayref = add_array_to_heap(count);
    stack_push(arrayref);

    ijvm.pc += 1;
}

void iaload(void)
{
    int32_t arrayref = stack_pop();
    int32_t index = stack_pop();

    stack_push(get_array_value(arrayref,index));
    
    ijvm.pc += 1;
}

void iastore(void)
{
    int32_t arrayref = stack_pop();
    int32_t index = stack_pop();
    int32_t value = stack_pop();

    value_to_array(arrayref, index, value);

    ijvm.pc += 1;
}

void netbind()
{
    int32_t port = stack_pop();
    int netref = network_listen(port);

    stack_push(netref);
    
    ijvm.pc += 1;
}

void netconnect()
{
    int32_t port = stack_pop();
    int32_t address = stack_pop();

    int netref = create_connection(address,port);
    stack_push(netref);

    ijvm.pc += 1;
}

void netin()
{
    int32_t netref = stack_pop();
    int32_t message = network_recieve(netref);

    stack_push(message);

    ijvm.pc += 1;
}

void netout()
{
    int32_t netref = stack_pop();
    int32_t message = stack_pop();

    network_send(netref,message);

    ijvm.pc += 1;
}

void netclose()
{
    int32_t netref = stack_pop();
    close_connection(netref);

    ijvm.pc += 1;
}

void in()
{
    int32_t charIn = getc(input);

    if (charIn != EOF){
        stack_push(charIn);
    } else {
        stack_push(0);
    }
    ijvm.pc += 1;
}

void out()
{
    int32_t val = stack_pop();
    fprintf(output, "%c",val);

    ijvm.pc += 1;
}
