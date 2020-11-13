#include <stdlib.h>
#include "ijvm.h"
#include "frame.h"

extern IJVM ijvm;

void init_main_frame()
{
    ijvm.currentFrame = malloc(sizeof(Frame));
    ijvm.currentFrame->frameSize = FRAME_SIZE;
    ijvm.currentFrame->items = malloc(sizeof(int32_t) * FRAME_SIZE);
    ijvm.currentFrame->prev = NULL;
}

Frame* new_frame(int frameSize, int argsNum)
{
    Frame *frame = malloc(sizeof(Frame));
    frame->items = (int32_t*)malloc(sizeof(int32_t) * (unsigned long)frameSize);
    for (int i = argsNum-1; i >= 0; i--){
        frame->items[i] = stack_pop();
    }
    frame->frameSize = frameSize;

    return frame;
}

void destroy_frames()
{
    if (ijvm.currentFrame == NULL) return;
    while (ijvm.currentFrame->prev != NULL){
        Frame *oldFrame = ijvm.currentFrame;
        ijvm.currentFrame = ijvm.currentFrame->prev;
        free_frame(oldFrame);
    }

    free(ijvm.currentFrame->items);
    free(ijvm.currentFrame);
}

void free_frame(Frame *frame)
{
    free(frame->items);
    frame->items = NULL;
    frame->prev = NULL;
    free(frame);
    frame = NULL;
}
