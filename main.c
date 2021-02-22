//Cem Gurbey 260788289
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kernel.h"

int main(int argc, const char *argv[]){
	int error = 0;
	boot();
    error = kernel();
    return error;
}