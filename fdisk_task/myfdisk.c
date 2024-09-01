#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

#define SECTOR_SIZE 512
#define BOOT_OFFSET 446
#define PRIMARY_MAX 4
#define KILO 1024
#define MEGA (1024*1024)
#define GIGA (1024*1024*1024)
#define MEGA_SEC 2048
#define GIGA_SEC 2097152

typedef struct {
    uint8_t status;
    uint8_t first_chs[3];
    uint8_t partition_type;
    uint8_t last_chs[3];
    uint32_t lba;
    uint32_t sector_count;
} PartitionEntry;

/* This funtion is used to get the type of the partition (primary, extended or logical) depending on its ID*/ 
char *getType(uint8_t type)
{
    if (type == 0x83)
	return "Linux";
    else if (type == 0x05)
	return "Extended";
}
/* This function id used to get the size of the partition in the right unit (K, M or G) depending on number of sector of each*/
uint32_t getsize(uint32_t sec_count, char *unit)
{
    if (sec_count < MEGA_SEC) {
	*unit = 'K';
	return ((uint32_t) (((uint64_t) sec_count * SECTOR_SIZE) / KILO));
    } else if (sec_count < GIGA_SEC) {
	*unit = 'M';
	return ((uint32_t) (((uint64_t) sec_count * SECTOR_SIZE) / MEGA));
    } else {
	*unit = 'G';
	return ((uint32_t)
		(((uint64_t) sec_count * SECTOR_SIZE) / GIGA));
    }
}

int main(int argc, char **argv)
{
    char buf[SECTOR_SIZE];
    char size_unit = 0;
    uint32_t size = 0;

    int fd = open(argv[1], O_RDONLY);
// read and parse the primary partitions entries in the table (the MBR)
    if (read(fd, buf, 512) == -1)
	perror("read");
    PartitionEntry *table_entry_ptr = (PartitionEntry *) & buf[BOOT_OFFSET];

    printf("%-10s %-6s %-9s %-6s %-8s %-8s %-7s %-4s\n", "Device",
	   "Boot", "Start", "End", "Sectors", "Size", "Id", "Type");

    for (int i = 0; i < 2; i++) {
    	size = getsize(table_entry_ptr[i].sector_count, &size_unit);
	printf("%s%d%6c%8u%8u%11u%5u%c%7X%10s\n",
	       argv[1],
	       i + 1,
	       table_entry_ptr[i].status == 0x80 ? '*' : ' ',
	       table_entry_ptr[i].lba,
	       table_entry_ptr[i].lba + table_entry_ptr[i].sector_count -
	       1, table_entry_ptr[i].sector_count,
	       size,
	       size_unit,
	       table_entry_ptr[i].partition_type,
	       getType(table_entry_ptr[i].partition_type));
    }
   
// check for extended partitions
    int e_index = -1;
    for (int i = 0; i < PRIMARY_MAX; i++) {
	if (table_entry_ptr[i].partition_type == 0x05) {
	    e_index = i;
	    break;
	}
    }

// parse extended partions EBRs if exist
    if (e_index != -1) {
	uint32_t ebr_lba = table_entry_ptr[e_index].lba;
	int i = 1;
	while (ebr_lba != 0) {
	    if(lseek(fd, ebr_lba * SECTOR_SIZE, SEEK_SET) == -1) // seek to the extended partition
	    perror("seek"); 
	    
	    if(read(fd, buf, SECTOR_SIZE) == -1)
	    perror("read ebr");
	    
	    table_entry_ptr = (PartitionEntry *) & buf[BOOT_OFFSET];
	    size = getsize(table_entry_ptr[0].sector_count, &size_unit);
	    printf("%s%d%6c%8u%8u%11u%5u%c%7X%10s\n", argv[1], (PRIMARY_MAX + i++),
		   table_entry_ptr[0].status == 0x80 ? '*' : ' ',
		   ebr_lba + table_entry_ptr[0].lba,
		   ebr_lba + table_entry_ptr[0].lba +
		   table_entry_ptr[0].sector_count - 1,
		   table_entry_ptr[0].sector_count, size, size_unit,
		   table_entry_ptr[0].partition_type,
		   getType(table_entry_ptr[0].partition_type));
		  

	    if (table_entry_ptr[1].lba != 0)
		ebr_lba = table_entry_ptr[1].lba + ebr_lba; // if second ebr exit, point to it
	    else
		ebr_lba = 0;
	}
    }
    return 0;
}
