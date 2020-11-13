#include <stdlib.h>
#include "ijvm.h"
#include <string.h>
#include "helpers.h"
#include "instructions.h"

IJVM ijvm;
FILE *output;
FILE *input;

int init_ijvm(char *binary_file)
{
    FILE *fp;
    
    fp = fopen(binary_file, "rb");
    if (fp == NULL) {
      return -1;
    }
    
    long bufferSize = get_file_size(fp);
    if (bufferSize < 20){ // not enough bytes
      return -1;
    }
    ijvm.buffer = (uint8_t *)malloc((unsigned long) bufferSize * sizeof(uint8_t));
    fread(ijvm.buffer, (unsigned long) bufferSize, 1, fp);
    fclose(fp);

    set_binary_blocks();

    ijvm.magicNumber = bytes_to_int(ijvm.buffer,0);
    if (ijvm.magicNumber != MAGIC_NUMBER){
        free(ijvm.buffer);
        ijvm.constantsPool = NULL;
        ijvm.textData = NULL;
        return -1;
    }
    ijvm.pc = 0;
    ijvm.isRunning = true;
    
    init_stack();
    init_main_frame();
    init_heap();
    
    set_input(stdin);
    set_output(stdout);

    return 0;
}

void set_binary_blocks()
{   
    ijvm.poolLength = bytes_to_int(ijvm.buffer,8);
    ijvm.constantsPool = &ijvm.buffer[12];
    
    int32_t textSizeOffset = 4 + 8 + ijvm.poolLength + 4;
    ijvm.textDataLength = bytes_to_int(ijvm.buffer,textSizeOffset);
    
    int32_t textDataOffset = textSizeOffset+4;
    ijvm.textData = &ijvm.buffer[textDataOffset];
}

void destroy_ijvm()
{
    destroy_frames();
    destroy_heap();
   
    free(ijvm.buffer);
    free(ijvm.stack.items);

    ijvm.constantsPool = NULL;
    ijvm.textData = NULL;
    output = NULL;
    input =  NULL;
}

void run()
{
  while (1){
    if( !(step()) || (ijvm.isRunning == false)){
      break;
    }
  }
}

bool step()
{
  if (ijvm.isRunning == false){
    return false;
  }
  if(execute_instruction(ijvm.textData[ijvm.pc])){
    return true;
  }
  ijvm.isRunning = false;
  return false;
}

void set_input(FILE *fp)
{
  input = fp;
}

void set_output(FILE *fp)
{
  output = fp;
}
