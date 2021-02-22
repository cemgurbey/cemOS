//Cem Gurbey 260788289
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ram.h"
#include "cpu.h"
#include "kernel.h"
#include "shell.h"
#include "memorymanager.h"
#include "ram.h"
#define DEFAULT_QUANTA 2


QUEUE_NODE *Head = NULL, *Tail = NULL, *Temp = NULL;
CPU* cpu;

void boot(){
    resetRAM();
    char rm_cmd[50];
    strcpy(rm_cmd, "rm -r BackingStore");
    system(rm_cmd);
    char mkdir_cmd[50];
    strcpy(mkdir_cmd, "mkdir BackingStore");
    system(mkdir_cmd);

}

int kernel(){
    printf("Kernel 2.0 loaded!\n");
	return shellUI();
}

void addToReady(PCB *pcb){
    QUEUE_NODE *newPCB = malloc(sizeof(QUEUE_NODE));
    newPCB->thisPCB = pcb;
    if(Head == NULL){
        Head = newPCB;
        Tail = newPCB;
    }else{
        Tail->next = newPCB;
        Tail = newPCB;
        Tail->thisPCB = pcb;
    }
}

void scheduler(){
    cpu = malloc(sizeof(CPU));
    cpu->quanta = DEFAULT_QUANTA;
    while((Head != NULL) && (Head != Tail->next)){
        int interruptStatusFlag = 0;
        PCB* removeHead = Head->thisPCB;
        if(removeHead->pageTable[removeHead->PC_page] == -1) PageFault(removeHead);
        cpu->IP = removeHead->pageTable[removeHead->PC_page];	    
        cpu->offset = removeHead->PC_offset;
        Temp = Head;
        int InstructionsToExecute = removeHead->lines_max - ((removeHead->PC_page) * MAX_OFFSET + removeHead->PC_offset);
        if(cpu->quanta < InstructionsToExecute){
        	interruptStatusFlag = runCPU(cpu->quanta);
        	if(interruptStatusFlag == 1){
        		removeHead->PC_offset = 0;
        		(removeHead->PC_page)++;
        		if(PageFault(removeHead) == 0) addToReady(removeHead);
        	}
        	else{
        		removeHead->PC_offset = cpu->offset;
        		removeHead->PC = (cpu->IP) * MAX_OFFSET + cpu->offset;
                addToReady(removeHead);
        	}
        }
        else{
        	interruptStatusFlag = runCPU(InstructionsToExecute);
            for(int i = 0; i < 10; i++){
            	if(removeHead->pageTable[i] != -1){
            		int index = (removeHead->pageTable[i]) * MAX_OFFSET;
                    removeFrame(index);
            	}
            }
            BackingStorageDelete(removeHead->pid);
            free(Temp);
        }
        Head = Head->next;
    }
    Head = NULL;
    Tail = Head;
    resetRAM();
}


