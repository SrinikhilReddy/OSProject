#include<sys/defs.h>
#include<sys/mem.h>
#include<sys/kprintf.h>
static freelist* head = NULL;
//TODO: Freelist as an array with fixed size will not work in NCS:106.
// Need to implement a better dynamically allocated list
// 
extern char kernmem;
static uint64_t count =0;
static uint64_t *pml4e, *pdpte,*pde,*pte;
static uint64_t pml4_idx,pdpt_idx,pd_idx,pt_idx;
static uint64_t viraddr;
void mem_map(smap_t* sm, uint64_t physbase, uint64_t physfree){
	uint64_t sbase = sm->base;
	uint64_t slim = sm->length;

	physfree = physfree + sizeof(pagelist);
	for(uint64_t ptr=sbase;ptr<(sbase+slim);ptr+=4096){
		if(ptr>physfree && ptr+4096<(sbase+slim)){
			pagelist[count].address = ptr;
			pagelist[count].next = head;
			pagelist[count].free = 1;
			head = &pagelist[count];
			count++;	
		}
		else{
			pagelist[count].address = ptr;
			pagelist[count].next = NULL;
			pagelist[count].free = 0;
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
/**
	Should return a pml4e address after creating a 4 level page level for a user process.
	Plan:
	Create an address space for a user process

**/
/*uint64_t newpage_u_proc(){
	
}*/
uint64_t allocate_page_for_process(){
	viraddr+=4096;	
	uint64_t ax = (uint64_t )allocate_page();	
	pt_idx = (viraddr >> 12 ) & 0x1FF;
	if(pd_idx == ((viraddr >> 21 ) & 0x1FF)){
		uint64_t s =  ( ax & 0xFFFFFFFFFFFFF000) | 3;
		pte[pt_idx] = s;
	}
	return viraddr;
}
void init_ia32e_paging(uint64_t physbase, uint64_t physfree){

	viraddr = (uint64_t)0xffffffff80000000;//(uint64_t)&kernmem;

	pml4_idx = (viraddr >> 39 ) & 0x1FF;
	pdpt_idx = (viraddr >> 30 ) & 0x1FF;
	pd_idx = (viraddr >> 21 ) & 0x1FF;
	pt_idx = (viraddr >> 12 ) & 0x1FF;

	pml4e = (uint64_t *)allocate_page();
	pdpte = (uint64_t *)allocate_page();
	pde = (uint64_t *)allocate_page();
	pte = (uint64_t *)allocate_page();
	pml4e[pml4_idx] = ((uint64_t)pdpte & 0xFFFFFFFFFFFFF000) | 3;
	pdpte[pdpt_idx] = ((uint64_t)pde & 0xFFFFFFFFFFFFF000) | 3;
	pde[pd_idx] = ((uint64_t)pte & (0xFFFFFFFFFFFFF000)) | 3;
	for(int j=0;physbase<physfree;j++,viraddr+=4096,physbase+=4096){
		pml4_idx = (viraddr >> 39 ) & 0x1FF;
		pdpt_idx = (viraddr >> 30 ) & 0x1FF;
		pd_idx = (viraddr >> 21 ) & 0x1FF;
		pt_idx = (viraddr >> 12 ) & 0x1FF;   

		if(pt_idx!=0){
			pte[pt_idx] = (physbase & 0xFFFFFFFFFFFFF000) | 3;
		}
		else{
			pte = ((uint64_t*)allocate_page());
			pde[pd_idx] = ((uint64_t)pte & (0xFFFFFFFFFFFFF000)) | 3;
			pte[pt_idx] = (physbase & 0xFFFFFFFFFFFFF000) | 3;		
		}
	}
	uint64_t cr3 = (uint64_t)pml4e;	
	// Temp Fix, we need a better approach

	viraddr+=4096;
	pt_idx = (viraddr >> 12 ) & 0x1FF;
	pte[pt_idx] = ( (uint64_t)pml4e & 0xFFFFFFFFFFFFF000) | 3;
	pml4e = (uint64_t*)viraddr;

	viraddr+=4096;
	pt_idx = (viraddr >> 12 ) & 0x1FF;
	pte[pt_idx] = ( (uint64_t)pdpte & 0xFFFFFFFFFFFFF000) | 3;
	pdpte = (uint64_t*)viraddr;

	viraddr+=4096;
	pt_idx = (viraddr >> 12 ) & 0x1FF;
	pte[pt_idx] = ( (uint64_t)pde & 0xFFFFFFFFFFFFF000) | 3;
	pde= (uint64_t*)viraddr;

	viraddr+=4096;
	pt_idx = (viraddr >> 12 ) & 0x1FF;
	pte[pt_idx] = ( (uint64_t)pte & 0xFFFFFFFFFFFFF000) | 3;
	pte = (uint64_t*)viraddr;

	__asm__ volatile("movq %0,%%cr3"::"r"(cr3));	
}

