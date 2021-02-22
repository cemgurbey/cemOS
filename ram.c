//Cem Gurbey 260788289
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ram.h"


void removeFrame(int index){
    ram[index] = NULL;
    ram[index + 1] = NULL;
    ram[index + 2] = NULL;
    ram[index + 3] = NULL;
}

void resetRAM(){
    for(int i = 0; i < 40; i++) ram[i] = NULL;
}
