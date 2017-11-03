#include<sys/defs.h>
#include<sys/mem.h>
#include<sys/kprintf.h>
static freelist* head = NULL;
//freelist* nu

void mem_map(smap_t* sm, uint64_t physbase, uint64_t physfree){
	uint64_t sbase = sm->base;
	uint64_t slim = sm->length;
	uint64_t freelist_space = physfree;
	uint64_t count = 0;
	physfree = physfree + (sizeof(struct freelist)*1024);
	for(uint64_t ptr=sbase;ptr<(sbase+slim);ptr+=4096){
		if(ptr>physfree){
			freelist* temp = (freelist *)freelist_space;
			temp->address = (uint64_t*)ptr;
			temp->next = head;
			head = temp;
			count++;		
			freelist_space = freelist_space+sizeof(freelist);
		}
	}	
/*	freelist* a = head;
	int l = 0;
	while(a->next!=NULL){
		l++;
		a = a->next;
		kprintf("====");
	}
	kprintf("Total free pages=%d,%d",count,l);
*/
}

void init_ia32e_paging(){

	//Following the convention described in Intel specification of IA32e type paging.
//	uint64_t pml4e[512], pdpte[512],pde[512],pte[512];
		
}
