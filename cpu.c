//Cem Gurbey 260788289
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"
#include "ram.h"
#include "cpu.h"

int runCPU(int quanta){
    while(quanta > 0){
    	int address = (cpu->IP) * MAX_OFFSET +(cpu->offset);
        strcpy(cpu->IR, ram[address]);
        interpret(cpu->IR);
        cpu->offset++;
        if(cpu->offset >= MAX_OFFSET){ 
            return 1;
        }
        quanta--;
    }
    return 0;
}
