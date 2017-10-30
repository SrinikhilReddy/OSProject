#include<sys/defs.h>

extern void loadPageDirectory(unsigned int*);
extern void enablePaging();

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void loadPageDirectory(unsigned int* page_directory) {

	//set each entry to not present
	int i;
	for(i = 0; i < 1024; i++)
	{
    		// This sets the following flags to the pages:
    		//   Supervisor: Only kernel-mode can access them
    		//   Write Enabled: It can be both read from and written to
    		//   Not Present: The page table is not present
    		page_directory[i] = 0x00000002;
	}

	// holds the physical address where we want to start mapping these pages to.
	// in this case, we want to map these pages to the very beginning of memory.
	unsigned int i;
 
	//we will fill all 1024 entries in the table, mapping 4 megabytes
	for(i = 0; i < 1024; i++)
	{
    		// As the address is page aligned, it will always leave 12 bits zeroed.
    		// Those bits are used by the attributes ;)
    		first_page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
	}

	// attributes: supervisor level, read/write, present
	page_directory[0] = ((unsigned int)first_page_table) | 3;
}

//NOt sure where this code should be placed!
/*void enablePaging() {

	.text
	.globl loadPageDirectory
	loadPageDirectory:
	push %ebp
	mov %esp, %ebp
	mov 8(%esp), %eax
	mov %eax, %cr3
	mov %ebp, %esp
	pop %ebp
	ret

	.text
	.globl enablePaging
	enablePaging:
	push %ebp
	mov %esp, %ebp
	mov %cr0, %eax
	or $0x80000000, %eax
	mov %eax, %cr0
	mov %ebp, %esp
	pop %ebp
	ret

}*/
