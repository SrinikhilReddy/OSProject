#include<sys/defs.h>
#include<sys/mem.h>
#include<sys/kprintf.h>
static freelist* head = NULL;
//TODO: Freelist as an array with fixed size will not work in NCS:106.
// Need to implement a better dynamically allocated list
// 
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
			//			kprintf("=====PHYSBASEEEEE");
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

void init_ia32e_paging(uint64_t physbase, uint64_t physfree){

	//Following the convention described in Intel specification of IA32e type paging.
	uint64_t *pml4e, *pdpte,*pde,*pte;
	uint64_t pml4_idx,pdpt_idx,pd_idx,pt_idx;
	uint64_t viraddr = (uint64_t)0xffffffff80000000;//(uint64_t)&kernmem;
	
	kprintf("==============%p",viraddr);	
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
	physbase = 0;//xffffffff80000000;
	//	physfree = physfree + 0xffffffff80000000;
	pml4e[pml4_idx] = ((uint64_t)pdpte & 0xFFFFFFFFFFFFF000) | 7;
	pdpte[pdpt_idx] = ((uint64_t)pde & 0xFFFFFFFFFFFFF000) | 7;
	pde[pd_idx] = ((uint64_t)pte & (0xFFFFFFFFFFFFF000)) | 7;
	int i = 1;
	for(int j=0;physbase<physfree;viraddr+=4096,physbase+=4096,j++){
		if(physbase == 0x200000){
			kprintf("-----%p,%p\n",physbase,viraddr);		
		}
		 pml4_idx = (viraddr >> 39 ) & 0x1FF;
                        pdpt_idx = (viraddr >> 30 ) & 0x1FF;
                //      kprintf("==========%d",pd_idx);
                        pd_idx = (viraddr >> 21 ) & 0x1FF;
                //      kprintf("==========%d",pd_idx);
                        pt_idx = (viraddr >> 12 ) & 0x1FF;   

		if(j<512){
			//			kprintf("--%d",j);
			//			if(i>1){
			//			kprintf("==========%d",pd_idx);
			//			}
			pte[pt_idx] = (physbase & 0xFFFFFFFFFFFFF000) | 7;
//			kprintf("##%d",pt_idx);
		}
		else{
			//TODO : Handle this.
	//				kprintf("===========Overflow");
			pte = ((uint64_t*)allocate_page());
			//			pde[pd_idx+i] = ((uint64_t)pte & (0xFFFFFFFFFFFFF000)) | 7;
			j=0;
			i++;	
//			kprintf("==========%d %d",pd_idx,pt_idx);
//			viraddr = viraddr+4096;
//			pml4_idx = (viraddr >> 39 ) & 0x1FF;
//			pdpt_idx = (viraddr >> 30 ) & 0x1FF;
//			pd_idx = (viraddr >> 21 ) & 0x1FF;
//			pt_idx = (viraddr >> 12 ) & 0x1FF;
		//	if(pd_idx>51){
		//		pde = ((uint64_t*)allocate_page());
		//	}
			pde[pd_idx] = ((uint64_t)pte & (0xFFFFFFFFFFFFF000)) | 7;
			 pte[pt_idx] = (physbase & 0xFFFFFFFFFFFFF000) | 7;		
		}
		//	viraddr = viraddr + 0x1000;
//			pml4_idx = (viraddr >> 39 ) & 0x1FF;
//			pdpt_idx = (viraddr >> 30 ) & 0x1FF;
		//	kprintf("==========%d",pd_idx);
//			pd_idx = (viraddr >> 21 ) & 0x1FF;
		//	kprintf("==========%d",pd_idx);
//			pt_idx = (viraddr >> 12 ) & 0x1FF;	
	}
//	kprintf("**************%p\n",physbase);
	for(;pd_idx>=1;pd_idx--){
		kprintf("***%p",pde[pd_idx]);
	}//Video mem paging
	//	uint64_t* vde_mem = (uint64_t *)allocate_page();
	//	pde[pd_idx+1] = ((uint64_t)vde_mem & 0xFFFFFFFFFFFFF000)|7;
	//	vde_mem[0] = (0xb8000 &0xFFFFFFFFFFFFF000) | 7;
		uint64_t cr3 = (uint64_t)pml4e;
		__asm__ volatile("movq %0,%%cr3"::"r"(cr3));	
}

