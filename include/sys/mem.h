#ifndef _PHY_H
#define _PHY_H
#include<sys/process.h>
typedef struct freelist{
	uint64_t address;
	int free;
	struct freelist* next; 
}freelist;
struct freelist pagelist[75000]; 
typedef struct smap_t {
    uint64_t base, length;
    uint32_t type;
}__attribute__((packed)) smap_t;
uint64_t allocate_page();
uint64_t allocate_page_for_process();


void mem_map(smap_t* sm, uint64_t physbase, uint64_t physfree);
void init_ia32e_paging();
void init_pages_for_process(uint64_t vaddr_s,uint64_t vaddr_e,uint64_t* pml4);
uint64_t kmalloc(int size);
void copytables(task_struct* a,task_struct* b);
#endif
