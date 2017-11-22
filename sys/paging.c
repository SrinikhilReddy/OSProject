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
uint64_t kmalloc(int size){
	int no_pages = (size/4096)+1;
	uint64_t add = allocate_page();
	no_pages--;
	if(no_pages>0){
 	for(int i=0;i<no_pages;i++){
		allocate_page();
		}
	}
	return add;
}
uint64_t allocate_page_for_process(){
	viraddr+=4096;	
	uint64_t ax = (uint64_t )allocate_page();	
	pt_idx = (viraddr >> 12 ) & 0x1FF;
	if(pd_idx == ((viraddr >> 21 ) & 0x1FF)){
		uint64_t s =  ( ax & 0xFFFFFFFFFFFFF000) | 7;
		pte[pt_idx] = s;
	}
	return viraddr;
}

uint64_t allocate_pages_for_process(uint64_t vaddr,uint64_t phy_start,uint64_t phy_end){
	uint64_t *pml4,*pdpt,*pd,*pt;
	uint64_t idx1,idx2,idx3,idx4;

	pml4 = (uint64_t *)allocate_page();
        pdpt = (uint64_t *)allocate_page();
        pd = (uint64_t *)allocate_page();
        pt = (uint64_t *)allocate_page();	
	
	idx1 = (vaddr >> 39 ) & 0x1FF;
        idx2 = (vaddr >> 30 ) & 0x1FF;
        idx3 = (vaddr >> 21 ) & 0x1FF;
        idx4 = (vaddr >> 12 ) & 0x1FF;
	
	pml4[idx1] = ((uint64_t)pdpt & 0xFFFFFFFFFFFFF000) | 7;
        pdpt[idx2] = ((uint64_t)pd & 0xFFFFFFFFFFFFF000) | 7;
        pd[idx3] = ((uint64_t)pt & (0xFFFFFFFFFFFFF000)) | 7;
	
	for(;phy_start<phy_end;phy_start+=4096){
		idx1 = (vaddr >> 39 ) & 0x1FF;
                idx2 = (vaddr >> 30 ) & 0x1FF;
                idx3 = (vaddr >> 21 ) & 0x1FF;
                idx4 = (vaddr >> 12 ) & 0x1FF;

                if(idx4!=0){
                        pt[idx4] = (phy_start & 0xFFFFFFFFFFFFF000) | 7;
                }
                else{
                        pt = ((uint64_t*)allocate_page());
                        pd[idx3] = ((uint64_t)pt & (0xFFFFFFFFFFFFF000)) | 7;
                        pt[idx4] = (phy_start & 0xFFFFFFFFFFFFF000) | 7;
                }
		
	}
	return (uint64_t)pml4;
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
	pml4e[pml4_idx] = ((uint64_t)pdpte & 0xFFFFFFFFFFFFF000) | 7;
	pdpte[pdpt_idx] = ((uint64_t)pde & 0xFFFFFFFFFFFFF000) | 7;
	pde[pd_idx] = ((uint64_t)pte & (0xFFFFFFFFFFFFF000)) | 7;
	for(int j=0;physbase<physfree;j++,viraddr+=4096,physbase+=4096){
		pml4_idx = (viraddr >> 39 ) & 0x1FF;
		pdpt_idx = (viraddr >> 30 ) & 0x1FF;
		pd_idx = (viraddr >> 21 ) & 0x1FF;
		pt_idx = (viraddr >> 12 ) & 0x1FF;   

		if(pt_idx!=0){
			pte[pt_idx] = (physbase & 0xFFFFFFFFFFFFF000) | 7;
		}
		else{
			pte = ((uint64_t*)allocate_page());
			pde[pd_idx] = ((uint64_t)pte & (0xFFFFFFFFFFFFF000)) | 7;
			pte[pt_idx] = (physbase & 0xFFFFFFFFFFFFF000) | 7;		
		}
	}
	uint64_t cr3 = (uint64_t)pml4e;	
	// Temp Fix, we need a better approach

	viraddr+=4096;
	pt_idx = (viraddr >> 12 ) & 0x1FF;
	pte[pt_idx] = ( (uint64_t)pml4e & 0xFFFFFFFFFFFFF000) | 7;
	pml4e = (uint64_t*)viraddr;

	viraddr+=4096;
	pt_idx = (viraddr >> 12 ) & 0x1FF;
	pte[pt_idx] = ( (uint64_t)pdpte & 0xFFFFFFFFFFFFF000) | 7;
	pdpte = (uint64_t*)viraddr;

	viraddr+=4096;
	pt_idx = (viraddr >> 12 ) & 0x1FF;
	pte[pt_idx] = ( (uint64_t)pde & 0xFFFFFFFFFFFFF000) | 7;
	pde= (uint64_t*)viraddr;

	viraddr+=4096;
	pt_idx = (viraddr >> 12 ) & 0x1FF;
	pte[pt_idx] = ( (uint64_t)pte & 0xFFFFFFFFFFFFF000) | 7;
	pte = (uint64_t*)viraddr;

	__asm__ volatile("movq %0,%%cr3"::"r"(cr3));	
}

