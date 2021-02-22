//Cem Gurbey 260788289
#ifndef CPU_H
#define CPU_H
#define MAX_OFFSET 4

int runCPU(int);

typedef struct CPU_t {
    int IP;
    int offset;
    char IR[1000];
    int quanta;
} CPU;

extern CPU *cpu;



#endif
