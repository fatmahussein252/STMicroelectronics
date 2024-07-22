This program aim is to see the different types of elf file (Executable, Relocatable and Dynamically linked) through running c file (task1.c) that calls another c file (sum.c).

used commands:

- to generate the symbol table of the main file (No dynamic linking):
	> gcc -static task1.c -o task1.o
	> readelf -s /home/fatma/STM/session_2/task1/task1.o

- to generate the symbol table of the called file (sum.c):
	> gcc -c sum.c -o sum.o
	> readelf -s /home/fatma/STM/session_2/task1/sum.o

- to generate the symbol table of the final executable file (With dynamic linking):
	> gcc task1.c
	> readelf -s /home/fatma/STM/session_2/task1/a.out


outputs screenshots are included in the directory.
