#include<sys/defs.h>
#include<sys/mem.h>
freelist* head = NULL;

void mem_map(smap_t* sm, uint64_t physbase, uint64_t physfree){
			
}
void init_ia32e_paging(){

	//Following the convention described in Intel specification of IA32e type paging.
	uint64_t pml4e[512], pdpte[512],pde[512],pte[512];
		
}
