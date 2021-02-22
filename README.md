# cemOS

Authered by Firat Cem Gurbey

you can run the cemOS on any MacOs/Linux command line!


Diclaimer:
If you are getting any type of error or SegFault while running the assignment please comment/uncomment the 23rd line of shell.c
shell_memory_destory(); --> COMMENT/UNCOMMENT HERE 


How To run:
gcc -c shell.c interpreter.c shellmemory.c kernel.c cpu.c pcb.c ram.c memorymanager.c main.c
gcc -o assignment3 shell.o interpreter.o shellmemory.o kernel.o cpu.o pcb.o ram.o memorymanager.o main.o
./assignment3 < TESTFILE.txtS
