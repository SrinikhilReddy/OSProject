#include<sys/defs.h>
#include<sys/mem.h>
#include<sys/kprintf.h>
static freelist* head = NULL;
//TODO: Freelist as an array with fixed size will not work in NCS:106.
// Need to implement a better dynamically allocated list
// 
extern char kernmem;
static uint64_t count =0;
  static      uint64_t *pml4e, *pdpte,*pde,*pte;
  static      uint64_t pml4_idx,pdpt_idx,pd_idx,pt_idx;
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
//uint64_t allocate_for_p(uint64_t addr){
	
	//uint64_t ax =  ((uint64_t*)allocate_page());	
	//pde[pd_idx] = ((uint64_t)ax & (0xFFFFFFFFFFFFF000)) | 7;
	//pte[pt_idx] = (addr & 0xFFFFFFFFFFFFF000) | 7;	
//}
void init_ia32e_paging(uint64_t physbase, uint64_t physfree){

	//Following the convention described in Intel specification of IA32e type paging.
//	uint64_t *pml4e, *pdpte,*pde,*pte;
//	uint64_t pml4_idx,pdpt_idx,pd_idx,pt_idx;
	uint64_t viraddr = (uint64_t)0xffffffff80000000;//(uint64_t)&kernmem;

	pml4_idx = (viraddr >> 39 ) & 0x1FF;
	pdpt_idx = (viraddr >> 30 ) & 0x1FF;
	pd_idx = (viraddr >> 21 ) & 0x1FF;
	pt_idx = (viraddr >> 12 ) & 0x1FF;

	pml4e = (uint64_t *)allocate_page();
	pdpte = (uint64_t *)allocate_page();
	pde = (uint64_t *)allocate_page();
	pte = (uint64_t *)allocate_page();
	kprintf("-----%p",pml4e);
//	physbase = 0;//xffffffff80000000;
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
	kprintf("================%p",physbase);
	uint64_t cr3 = (uint64_t)pml4e;
	__asm__ volatile("movq %0,%%cr3"::"r"(cr3));	
}

