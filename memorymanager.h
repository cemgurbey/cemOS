//Cem Gurbey 260788289
#include <stdio.h>
#include "pcb.h"

#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

int launcher(FILE *p);
int PageFault(PCB *pcb);
int BackingStorageDelete(int pid);

#endif
