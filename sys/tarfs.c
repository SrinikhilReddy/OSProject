#include <sys/tarfs.h>
#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/elf64.h>
#include <sys/file.h>
#include <string.h>
#include <stdlib.h>

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
	if(elf_hdr->e_phoff > 0){
		phdr = add + (elf_hdr->e_phoff);
		struct Elf64_Phdr* phdr1 = (struct Elf64_Phdr*)phdr;
		if(phdr1 != NULL){
			kprintf("Whatever\n");
		}
		kprintf("%d\n",(phdr1->p_type));
	}
}
void init_tarfs()
{
	struct posix_header_ustar *header = (struct posix_header_ustar *)&_binary_tarfs_start;
	int i = 0;
	char* address = &_binary_tarfs_start;
	while(address< &_binary_tarfs_end){	
		kprintf("%s ",header->name);
		unsigned int size = getsize(header->size);
		kprintf("Size: %d \n",i, size);
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

struct file_t* open_tarfs(char* file_path, int flags)
{

	struct file_t *f;
	struct posix_header_ustar* h = NULL;
	int i=0;
	for(i=0; i<32 && headers[i]!=NULL; i++)
	{
		if(strcmp(headers[i]->name,file_path)==0)
		{
			h = headers[i];
			break;
		}
	}
	f = kmalloc(sizeof(struct file_t));
	//f->offset = (off_t)headers[i+1];
	f->flags = flags;
	f->size = (uint64_t)h->size;
	f->address = (uint64_t)h;
	return f;
}
