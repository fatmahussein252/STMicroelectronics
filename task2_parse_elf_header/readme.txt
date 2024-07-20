This is an ELF parser application.
It parses an ELF file and prints the following dependent on the option entered:

    - The ELF header content exactly as the output of the command "readelf -h". (using option: -p)
	Note: the code supports (Intel 80386, Intel 80860 and x86-64) machines only. 

    - The name, size and type of all the ELF section headers (if exists). (using option: -s)

to get the elf header, write the commands:
> gcc readelf.c (or gcc -static readelf.c)
> ./a.out <real path of the elf file you have> -p  # you may replace "a.out" with the name of the executable file you specified.

to get the section header, write the commands:
> gcc readelf.c (or gcc -static readelf.c)
> ./a.out <real path of the elf file you have> -s


Output of reading the elf header of the executable file of this code is included in the directory.
