//Cem Gurbey 260788289
#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include "shellmemory.h"

int shellUI(){
    printf("Welcome to the Cem shell!\n"
           "Version 3.0 Updated April 2020\n");

    memoryinit();

    while (!feof(stdin)){
        printf("$ ");
        fflush(stdout);
        size_t linecap = 0;
        char *line = NULL;
        if (getline(&line, &linecap, stdin) == -1) break;
        (void)interpret(line);
        free(line);
    }
    shell_memory_destory();

    return 0;
}
