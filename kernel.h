//Cem Gurbey 260788289
#ifndef KERNEL_H
#define KERNEL_H
#include "pcb.h"

typedef struct QUEUE_NODE_t {
    PCB *thisPCB;
    struct QUEUE_NODE_t *next;
} QUEUE_NODE;

extern QUEUE_NODE *Head;
extern QUEUE_NODE *Tail;

void scheduler();
void addToReady(PCB *pcb);

void boot();
int kernel();

#endif
