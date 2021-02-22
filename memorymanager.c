//Cem Gurbey 260788289
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "memorymanager.h"
#include "ram.h"
#include "kernel.h"
#include "pcb.h"

#define PAGE_SIZE 4
char *ram[40];
int current_PID = 0;


int isEmpty(int frame){
	if(ram[frame] == NULL && ram[frame + 1] == NULL && ram[frame + 2] == NULL && ram[frame + 3] == NULL) {
		return 1;
	}
	return 0;
}

//updates the page table
void newPT(PCB *p, int page, int frame, int Vixtim){
	if(Vixtim != 0){
		QUEUE_NODE *pointer = Head;
		while((pointer != NULL) && (pointer != Tail->next)){
			PCB *thisPCB = pointer->thisPCB;
			for(int i = 0; i < 10; i++){ 
				if((thisPCB->pageTable[i]) == frame){ 
					thisPCB->pageTable[i] = -1;
				}
				pointer = pointer->next;
			}
		}
	}
	p->pageTable[page] = frame;

}


int findFrame(){
	for(int i=0; i<=36; i+=4){ 
		if(isEmpty(i)){ 
			return i/4;
		}
	}
	return -1;
}

int pcbVictim(PCB *p, int index){
	int length = sizeof(p->pageTable)/sizeof(int);
	for(int i = 0; i <= length; i++){	
		if(p->pageTable[i] == index){ 
			return 1;
		}
	}
	return 0;
}

int NumberOfLines(FILE *f){
	int lines = 0;
	fseek(f, 0, SEEK_SET);
    while(!feof(f)){
		size_t linecap = 0;
        char *line = NULL;
        getline(&line, &linecap, f);
    	lines++;
    }
    return lines;
}

int pidfPath(char** file, int pid){
	char fName[20];
	char fPath[20] = "BackingStore/";
	sprintf(fName, "%d", pid);
	strcat(fPath, fName);
	strcat(fPath, ".txt");
	*file = strdup(fPath);
	return 0;
}

int copyFile(FILE* ptr, FILE* ptrTwo){
	char buffer[1000];
	while(fgets(buffer, sizeof buffer, ptr) != NULL) {
		if(buffer[0] == '\0' || buffer[0] == '\n' || buffer[0] == ' ')
			continue;
		fputs(buffer, ptrTwo);
	}

    fclose(ptr);
    fclose(ptrTwo);
    return 0;
}

int NumberOfPages(FILE *f){
	int lines = NumberOfLines(f);
    int Count = lines / PAGE_SIZE;
    if(lines % PAGE_SIZE != 0) Count++;

    return Count;
}

int BackingStorageDelete(int pid){
	char* fPath;
	char rm_cmd[100];
	pidfPath(&fPath, pid);
    strcpy(rm_cmd, "rm ");
    strcat(rm_cmd, fPath);
    system(rm_cmd);
    return 0;
}


int newFile(char** file){
	char* fPath;
	char touch_cmd[100];
	pidfPath(&fPath, current_PID);
	strcpy(touch_cmd, "touch ");
	strcat(touch_cmd, fPath);
	system(touch_cmd);
	*file = strdup(fPath);
	return 0;
}

int findVictim(PCB *p){
	srand(time(0));
	int victimIndex = (rand() % 40) / PAGE_SIZE;
	while(pcbVictim(p, victimIndex)) victimIndex = (victimIndex + 1) % 10;
	return victimIndex;
}


int loadPage(int page, FILE *f, int frame){
	fseek(f, 0, SEEK_SET);
	int offset = 0, lineCount = 0;
	int lineTarget = page * PAGE_SIZE;

    while(!feof(f)){
		size_t linecap = 0;
        char *line = NULL;
        getline(&line, &linecap, f);
    	if(lineCount >= lineTarget && lineCount < lineTarget + PAGE_SIZE){
    		ram[frame * PAGE_SIZE + offset]=strdup(line);
    		offset++;
    	}
    	lineCount++;
    }
	return 0;
}

int PageFault(PCB *pcb){
	int error=0;
	int VictimBool=0;
	if(pcb->PC_page >= pcb->pages_max){ 
		return 1;
	}
	if(pcb->pageTable[pcb->PC_page] == -1){
		int frame = findFrame();
		if(frame == -1)	{
			frame = findVictim(pcb);
			VictimBool = 1;
		}
		char* fPath;
		pidfPath(&fPath, pcb->pid);
		FILE* f = fopen(fPath, "r");
		error = loadPage(pcb->PC_page, f, frame);
		if(error != 0) return error;
		newPT(pcb, pcb->PC_page, frame, VictimBool);
	}
	pcb->PC = (pcb->pageTable[pcb->PC_page]) * PAGE_SIZE + pcb->PC_offset;

	return error;
}


int StartPage(PCB *pcb, FILE* f, int totalPages){
	int errorCode=0, VictimBool = 0;
	int frame = findFrame();
	if(frame == -1){
		frame = findVictim(pcb);
		VictimBool = 1;
	}
	errorCode = loadPage(0, f, frame);
	if(errorCode!=0) return errorCode;
	newPT(pcb, 0, frame, VictimBool);
	if(totalPages > 1){
		frame = findFrame();
		if(frame == -1)	{
			frame = findVictim(pcb);
			VictimBool = 1;
		}
		errorCode = loadPage(1, f, frame);
		if(errorCode!=0){ 
			return errorCode;
		}
		newPT(pcb, 1, frame, VictimBool);
	}
	return errorCode;
}

//ERROR: -5 execution error
int launcher(FILE *ptr){
	char* file;
	int errorCode = 0;
	newFile(&file);
	FILE *ptrTwo = fopen(file, "w");
	copyFile(ptr, ptrTwo);
	FILE *f = fopen(file, "r");
	int numberOfLines = NumberOfLines(f);
	int numberOfPages = NumberOfPages(f);

	if(numberOfPages > 10) {
		BackingStorageDelete(current_PID);
		return -5;
	}

    PCB* pcb = makePCB(current_PID, numberOfPages, numberOfLines);
    errorCode = StartPage(pcb, f, numberOfPages);
    pcb->PC = pcb->pageTable[0];
    addToReady(pcb);
    current_PID++;

	return errorCode;
}