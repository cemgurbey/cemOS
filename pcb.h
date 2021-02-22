//Cem Gurbey 260788289
#ifndef PCB_H
#define PCB_H

typedef struct PCB_t {
	int pid;
	int lines_max;
    int PC;
    int pageTable[10];
    int PC_page;
    int PC_offset;
    int pages_max;
} PCB;

PCB* makePCB(int pid, int pages_max, int lines_max);

#endif
