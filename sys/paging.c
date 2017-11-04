#include<sys/defs.h>
#include<sys/mem.h>
#include<sys/kprintf.h>
static freelist* head = NULL;
//freelist* nu
//struct freelist pagelist[1000];
extern char kernmem;
static uint64_t count =0;
void mem_map(smap_t* sm, uint64_t physbase, uint64_t physfree){
	uint64_t sbase = sm->base;
	uint64_t slim = sm->length;
	//uint64_t freelist_space = physfree;
	//uint64_t count = 0;
	physfree = physfree + sizeof(pagelist);
	for(uint64_t ptr=sbase;ptr<(sbase+slim);ptr+=4096){
		if(ptr>physfree){
//			freelist* temp = (freelist *)freelist_space;
//			temp->address =ptr;
//			temp->next = head;
//			head = temp;
			pagelist[count].address = ptr;
			pagelist[count].next = head;
			pagelist[count].free = 1;
			head = &pagelist[count];
			count++;	
		//	freelist_space = freelist_space+sizeof(freelist);
		}
		else{
			pagelist[count].address = ptr;
                        pagelist[count].next = NULL;
                    pagelist[count].free = 0;
                        //head = &pagelist[count];
			count++;
		}
	}	
}
uint64_t allocate_page(){
	freelist* temp = head;
	temp->free=1;	
	head = head->next;
	
	return temp->address;
}	
void init_ia32e_paging(){

	//Following the convention described in Intel specification of IA32e type paging.
	uint64_t *pml4e, *pdpte,*pde,*pte;
	uint64_t pml4_idx,pdpt_idx,pd_idx,pt_idx;
	uint64_t viraddr = (uint64_t)&kernmem;
	
	pml4_idx = (viraddr >> 39 ) & 0x1FF;
	pdpt_idx = (viraddr >> 30 ) & 0x1FF;
	pd_idx = (viraddr >> 21 ) & 0x1FF;
	pt_idx = (viraddr >> 12 ) & 0x1FF;
	
	pml4e = (uint64_t *)allocate_page();
	pdpte = (uint64_t *)allocate_page();
	pde = (uint64_t *)allocate_page();
	pte = (uint64_t *)allocate_page();
	
	kprintf("------------------------%p %d\n",pml4e,pml4_idx);			
	kprintf("------------------------%p %d\n",pdpte,pdpt_idx);
	kprintf("------------------------%p %d\n",pde,pd_idx);
	kprintf("------------------------%p %d\n",pte,pt_idx);

	pml4e[pml4_idx] = ((uint64_t)pdpte & 0xFFFFFFFFFFFFF000) | 7;
	pdpte[pdpt_idx] = ((uint64_t)pde & 0xFFFFFFFFFFFFF000) | 7;
	pde[pd_idx] = ((uint64_t)pte & (0xFFFFFFFFFFFFF000)) | 7;
	
	 	
}

