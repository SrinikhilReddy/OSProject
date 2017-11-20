#include <sys/tarfs.h>
#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/elf64.h>

struct posix_header_ustar *headers[32];

unsigned int getsize(const char *in)
{

	unsigned int size = 0;
	unsigned int j;
	unsigned int count = 1;

	for (j = 11; j > 0; j--, count *= 8)
		size += ((in[j - 1] - '0') * count);

	return size;

}

void read_elf(uint64_t add){
	struct Elf64_Ehdr* elf_hdr = (struct Elf64_Ehdr*)add;
	uint64_t phdr;
	phdr = add + (elf_hdr->e_phoff);
	struct Elf64_Phdr* phdr1 = (struct Elf64_Phdr*)phdr;
	if(phdr1 != NULL){
		kprintf("Whatever\n");
	}
//	kprintf("%p\n",(phdr1->p_type));
}
void init_tarfs()
{
	struct posix_header_ustar *header = (struct posix_header_ustar *)&_binary_tarfs_start;
	int i = 0;
	char* address = &_binary_tarfs_start;
	while(address< &_binary_tarfs_end){	
		kprintf("%s ",header->name);
		unsigned int size = getsize(header->size);
		kprintf("%d, %d \n",i, size);
		headers[i++] = header;
		if(size!=0){
		read_elf((uint64_t)(header+1));
		}
		address += ((size / 512) + 1) * 512;

		if (size % 512)
			address += 512;

		header = (struct posix_header_ustar *)address;
	}

}
