#include<stdio.h>
#include <elf.h>
#include<unistd.h>

void elf_header(Elf64_Ehdr,FILE*);
void section_header(Elf64_Shdr*,FILE*,Elf64_Ehdr);

int main(int argc, char** argv)
{
	int option;
	const char* optstring = "ps?";
	FILE* myelfile  = fopen(argv[1],"r");
	Elf64_Ehdr myelfheader;
	Elf64_Shdr *mySheader;

	
	while ((option = getopt(argc, argv, optstring)) != -1)
   	{
		
		switch (option)
		{
			case 'p': elf_header(myelfheader,myelfile);break;
			case 's': section_header(mySheader,myelfile,myelfheader);break;
			case '?': printf("Unknown option\n");break;

		}
	}
	return 0;
}	

void elf_header(Elf64_Ehdr myelfheader, FILE* myelfile)
{
	fread(&myelfheader,sizeof(myelfheader),1,myelfile);
	printf("ELF Header:\n");
	printf("  %-9s","Magic:");
	for (int i = 0; i<sizeof(myelfheader.e_ident); i++)
	{
		printf("%02x ",myelfheader.e_ident[i]);
	}	
	printf("\n");
	printf("  %-35s","Class:");
	switch (myelfheader.e_ident[EI_CLASS])
	{
		case ELFCLASSNONE: printf("Invalid Class\n"); break; 
		case ELFCLASS32: printf("ELF32\n"); break;
		case ELFCLASS64: printf("ELF64\n"); break;
	}
	printf("  %-35s","Data:");
        switch (myelfheader.e_ident[EI_DATA])
        {
		case ELFDATANONE: printf("Unknown data format\n"); break;
                case ELFDATA2LSB: printf("2's complement, little-endian\n"); break;
           	case ELFDATA2MSB: printf("2's complement, big-endian\n"); break;
        }
	printf("  %-35s","Version:");
        switch (myelfheader.e_ident[EI_VERSION])
        {
                case EV_NONE: printf("Invalid Version\n"); break;
                case EV_CURRENT: printf("%d (current)\n", EV_CURRENT); break;
        }
	printf("  %-35s","OS/ABI:");
        switch (myelfheader.e_ident[EI_OSABI])
        {
		case ELFOSABI_SYSV: printf("UNIX - System V\n");break;
                case ELFOSABI_HPUX:printf("HP-UX ABI\n");break;
		case ELFOSABI_NETBSD:printf("NetBSD ABI\n");break;
		case ELFOSABI_LINUX:printf("UNIX - GNU (Linux ABI)\n");break;
                case ELFOSABI_SOLARIS:printf("Solaris ABI\n");break;
                case ELFOSABI_IRIX:printf("IRIX ABI\n");break;
		case ELFOSABI_FREEBSD:printf("FreeBSD ABI\n");break;
                case ELFOSABI_TRU64:printf("TRU64 UNIX ABI\n");break;
		case ELFOSABI_ARM:printf("ARM architecture ABI\n");break;
		case ELFOSABI_STANDALONE:printf("Stand-alone (embedded) AB\n");break;
        }
	printf("  %-35s%d\n","ABI Version:",0);
        printf("  %-35s","Type:");
        switch (myelfheader.e_type)
	{
		case ET_NONE: printf("Unknown type\n");break;
		case ET_REL:  printf("REL (Relocatable file)\n");break;
              	case ET_EXEC: printf("EXEC (Executable file)\n");break;
		case ET_DYN:  printf("DYN (Position-Independent Executable file)\n");break;
                case ET_CORE: printf("Core file\n");break;

	}	
	printf("  %-35s","Machine:");
        switch (myelfheader.e_machine)
        {
		case EM_NONE: printf("Unknown machine\n");break;
                case EM_386: printf("Intel 80386\n");break;
                case EM_860: printf("Intel 80860\n");break;
                case EM_X86_64: printf("Advanced Micro Devices X86-64\n");break;

        }
	printf("  %-35s","Version:");
        switch (myelfheader.e_version)
        {
		case EV_NONE:printf("Invalid version\n");break;
                case EV_CURRENT:printf("0x%x\n",EV_CURRENT);break;

        }
	printf("  %-35s0x%lx\n","Entry point address:",myelfheader.e_entry);
        printf("  %-35s%li (bytes into file)\n","Start of program headers:",myelfheader.e_phoff);
        printf("  %-35s%li (bytes into file)\n","Start of section headers:",myelfheader.e_shoff);
        printf("  %-35s0x%x\n","Flags:",myelfheader.e_flags);
        printf("  %-35s%d (bytes)\n","Size of this header:",myelfheader.e_ehsize);
        printf("  %-35s%d (bytes)\n","Size of program headers:",myelfheader.e_phentsize);
        printf("  %-35s%d\n","Number of program headers:",myelfheader.e_phnum);
        printf("  %-35s%d (bytes)\n","Size of section headers:",myelfheader.e_shentsize);
        printf("  %-35s%d\n","Number of secion headers:",myelfheader.e_shnum);
        printf("  %-35s%d\n","Section header string table index:",myelfheader.e_shstrndx);


	fclose(myelfile);
}

void section_header(Elf64_Shdr *mySheader, FILE* myelfile,Elf64_Ehdr myelfheader)
{
	fread(&mySheader,sizeof(mySheader),1,myelfile);
	printf("Actually, I really couldn't understand how to get section header names\nand i have no time for this part now. but i really struggled to undersand\nthe getopt() function and how to use it so i left it. وبارك الله فيما رزق ");
	

}


