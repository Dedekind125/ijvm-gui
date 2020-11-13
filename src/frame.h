#ifndef FRAME_H
#define FRAME_H

#include "ijvm.h"
#define FRAME_SIZE  64   /* Frame size of the first (main frame) */

typedef struct Frame
{
  struct Frame  *prev;
  int32_t       *items;
  
  int           frameSize;
  int           oldpc;
  int           oldsp;
} Frame;

/**
 * Initializes the main(first) frame of the ijvm machine.
 * For this frame the allocated space if defined as FRAME_SIZE
 **/
void init_main_frame(void);

/**
 * Creates a new frame.
 * @param frameSize the size of the new frame
 * @param argsNum   the number of the method arguments
 * @returns    a pointer to the new frame. 
 **/
Frame* new_frame(int frameSize, int argsNum);

/**
 * Free all the memory allocations what were allocated to
 * store the frames of the ijvm machine.
 **/
void destroy_frames(void);

/**
 * Frees the memory allocations of a specific frame
 * @param *frame a pointer to the frame to be destroyed.
 **/
void free_frame(Frame *frame);

#endif
