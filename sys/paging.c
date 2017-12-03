#include<sys/defs.h>
#include<sys/process.h>
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
static uint64_t k_cr3 =0;
void mem_map(smap_t* sm, uint64_t physbase, uint64_t physfree){
	uint64_t sbase = sm->base;
	uint64_t slim = sm->length;
	freelist* last = NULL;
	physfree = physfree + sizeof(pagelist);
	for(uint64_t ptr=sbase;ptr<(sbase+slim);ptr+=4096){
		if(ptr>physfree && ptr+4096<(sbase+slim)){
			if(head == NULL){
				pagelist[count].address = ptr;
				pagelist[count].next = head;
				pagelist[count].free = 1;
				head = &pagelist[count];
				last = head;	
				count++;   				
			}
			else{
				pagelist[count].address = ptr;
				pagelist[count].next = NULL;
				pagelist[count].free = 1;
				last->next = &pagelist[count];
				last = &pagelist[count];
				count++;
			}	
		}
		else{
			pagelist[count].address = ptr;
			pagelist[count].next = NULL;
			pagelist[count].free = 0;
			count++;
		}
	}	
}
void free(uint64_t add){
    for (int i = 0; i < count; ++i) {
        if(pagelist[i].address == add){
            pagelist[i].free = 1;
            pagelist[i].next = head;
            head = &pagelist[i];
        }
    }
}
void switchtokern(){
__asm__ volatile("movq %0,%%cr3;"::"r"((uint64_t)k_cr3));// - 0xffffffff80000000):);
}
uint64_t allocate_page(){
	freelist* temp = head;
	if(temp == NULL){
		kprintf("Trouble Land\\\\\\/n");
	}
	temp->free=0;
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
	uint64_t add = allocate_page_for_process();
	no_pages--;
	if(no_pages>0){
		for(int i=0;i<no_pages;i++){
			allocate_page_for_process();
		}
	}
	return add;
}
void printpml4(uint64_t* p4){
        for(int i = 0;i<511;i++){
                if( (p4[i]&1)){
                        kprintf("PML4 id:%d val:%p \n",i,(p4[i] & 0xFFFFFFFFFFFFF000));
                        uint64_t* p3 = (uint64_t *)(p4[i] & 0xFFFFFFFFFFFFF000);
			p3 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p3);
                        for(int j=0;j<512;j++){
                                if( (p3[j]&1)){
                                         kprintf("P3 id:%d val:%p \n",j,(p3[j] & 0xFFFFFFFFFFFFF000));
                                         uint64_t* p2 = (uint64_t *)(p3[j] & 0xFFFFFFFFFFFFF000);
				                                         p2 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p2);

                                          for(int k=0;k<512;k++){
                                                 if( (p2[k]&1)){
                                                    kprintf("P2 id:%d val:%p \n",k,(p2[k] & 0xFFFFFFFFFFFFF000));
                                                    uint64_t* p1 = (uint64_t *)(p2[k] & 0xFFFFFFFFFFFFF000);
		                                    p1 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p1);
                                                    for (int l = 0; l < 512; ++l)
                                                    {
                                                       if ((p1[l]&1))
                                                       {
                                                            kprintf("P1 id:%d val:%p \n",l,(p1[l] ));//& 0xFFFFFFFFFFFFF000));
                                                       }
                                                    }

                                                 }
                                          }
                                }   
                        }   
                }   
        }   
}
uint64_t* getPTE(uint64_t v){ 
        uint64_t* p4 = (uint64_t*)(r->pml4e + 0xffffffff80000000);
        int id4 = (v >> 39 ) & 0x1FF;
    	uint64_t* p3 = (uint64_t*)(p4[id4] + 0xffffffff80000000);
    	int id3 = (v >> 30 ) & 0x1FF;
    	uint64_t* p2 = (uint64_t*)(p3[id3] + 0xffffffff80000000);
    	int id2 = (v >> 21 ) & 0x1FF;
    	return (uint64_t*)(p2[id2] + 0xffffffff80000000);

}

void map(uint64_t vaddr_s, uint64_t phy){
	int id1 = (vaddr_s >> 39 ) & 0x1FF;
	if(!(pml4e[id1] & 1)){
		uint64_t* p3 = (uint64_t *)allocate_page();
		
		pml4e[id1] = ((uint64_t)p3 & 0xFFFFFFFFFFFFF000) | 3;

		p3 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p3);

		int id2 = (vaddr_s >> 30 ) & 0x1FF;
		uint64_t* p2 = (uint64_t *)allocate_page();
		p3[id2] = ((uint64_t)p2 & 0xFFFFFFFFFFFFF000) | 3;

		p2 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p2);

		int id3 = (vaddr_s >> 21 ) & 0x1FF;
		uint64_t* p1 = (uint64_t *)allocate_page();
		p2[id3] = ((uint64_t)p1 & 0xFFFFFFFFFFFFF000) | 3;

		p1 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p1);

		int id4 = (vaddr_s >> 12 ) & 0x1FF;
		p1[id4] =  ((uint64_t)phy & 0xFFFFFFFFFFFFF000) | 3;
		return ;
	}
	else{
		uint64_t* p3 = (uint64_t *)(pml4e[id1] & 0xFFFFFFFFFFFFF000);
		int id2 =  (vaddr_s >> 30 ) & 0x1FF;
		p3 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p3);
		if( !(p3[id2] & 1)){	
			uint64_t* p2 =(uint64_t *) allocate_page();
			p3[id2] = ((uint64_t)p2 & 0xFFFFFFFFFFFFF000) | 3;

			p2 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p2);

			int id3 = (vaddr_s >> 21 ) & 0x1FF;
			uint64_t* p1 = (uint64_t *)allocate_page();
			p2[id3] = ((uint64_t)p1 & 0xFFFFFFFFFFFFF000) | 3;

			p1 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p1);

			int id4 = (vaddr_s >> 12 ) & 0x1FF;
			p1[id4] =  ((uint64_t)phy & 0xFFFFFFFFFFFFF000) | 3;
			return;
		}
		else{
			uint64_t* p2 = (uint64_t *)(p3[id2] &0xFFFFFFFFFFFFF000);
			int id3 =  (vaddr_s >> 21) & 0x1FF;
			p2 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p2);

			if( !(p2[id3] & 1)){
				uint64_t* p1 = (uint64_t *)allocate_page();
				p2[id3] = ((uint64_t)p1 & 0xFFFFFFFFFFFFF000) | 3;

				p1 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p1);

				int id4 = (vaddr_s >> 12 ) & 0x1FF;
				p1[id4] =  ((uint64_t)phy & 0xFFFFFFFFFFFFF000) | 3;
				return;
			}
			else{	
				uint64_t* p1 = (uint64_t *)(p2[id3] &0xFFFFFFFFFFFFF000);	
				int id4 = (vaddr_s >> 12 ) & 0x1FF;

				p1 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p1);

				p1[id4] = ((uint64_t)phy & 0xFFFFFFFFFFFFF000) | 3;
				return;
			}
		}
	}
}

uint64_t allocate_page_for_process(){
	uint64_t ax = (uint64_t )allocate_page();	
	viraddr = 0xffffffff80000000 + ax;
	map(viraddr,ax);
	return viraddr;
}
uint64_t getPhysical(uint64_t vr){
	return (uint64_t) *((uint64_t *)(*(uint64_t *)( *((uint64_t *)(pml4e[(vr>>39) & 0x1FF])+ ((vr>>30)&0x1FF)) + ((vr>>21)&0x1FF)) + ((vr>>12)&0x1FF)));
}
//void vmtophy(uint64_t* pml4, uint64_t vs){
	
//}
void init_pages_for_process(uint64_t vaddr_s, uint64_t phy, uint64_t* pml4){
	pml4[511] = (pml4e[511] & 0xFFFFFFFFFFFFF000) | 7;
	int id1 = (vaddr_s >> 39 ) & 0x1FF;
	if(!(pml4[id1] & 1)){
		uint64_t* p3 = (uint64_t *)allocate_page();
		pml4[id1] = ((uint64_t)p3 & 0xFFFFFFFFFFFFF000) | 7;
			
		p3 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p3);
//		uint64_t a = getPhysical((uint64_t)p3);
//		kprintf("((((((((((((((( %p\n",a);
		int id2 = (vaddr_s >> 30 ) & 0x1FF;
		uint64_t* p2 = (uint64_t *)allocate_page();
		p3[id2] = ((uint64_t)p2 & 0xFFFFFFFFFFFFF000) | 7;

		p2 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p2);
		int id3 = (vaddr_s >> 21 ) & 0x1FF;
		uint64_t* p1 = (uint64_t *)allocate_page();
		p2[id3] = ((uint64_t)p1 & 0xFFFFFFFFFFFFF000) | 7;

		p1 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p1);
		int id4 = (vaddr_s >> 12 ) & 0x1FF;
		p1[id4] =  ((uint64_t)phy & 0xFFFFFFFFFFFFF000) | 7;
		return ;
	}

	else{
		uint64_t* p3 = (uint64_t *)(pml4[id1] & 0xFFFFFFFFFFFFF000);
		int id2 =  (vaddr_s >> 30 ) & 0x1FF;
		p3 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p3);
		if( !(p3[id2] & 1)){
			uint64_t* p2 =(uint64_t *) allocate_page();
			p3[id2] = ((uint64_t)p2 & 0xFFFFFFFFFFFFF000) | 7;

			p2 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p2);
			int id3 = (vaddr_s >> 21 ) & 0x1FF;
			uint64_t* p1 = (uint64_t *)allocate_page();
			p2[id3] = ((uint64_t)p1 & 0xFFFFFFFFFFFFF000) | 7;

			p1 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p1);
			int id4 = (vaddr_s >> 12 ) & 0x1FF;
			p1[id4] =  ((uint64_t)phy & 0xFFFFFFFFFFFFF000) | 7;
			return;
		}
		else{
			uint64_t* p2 = (uint64_t *)(p3[id2] &0xFFFFFFFFFFFFF000);
			int id3 =  (vaddr_s >> 21) & 0x1FF;
			p2 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p2);
			if( !(p2[id3] & 1)){
				uint64_t* p1 = (uint64_t *)allocate_page();
				p2[id3] = ((uint64_t)p1 & 0xFFFFFFFFFFFFF000) | 7;

				p1 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p1);
				int id4 = (vaddr_s >> 12 ) & 0x1FF;
				p1[id4] =  ((uint64_t)phy & 0xFFFFFFFFFFFFF000) | 7;

				return;
			}
			else{
				uint64_t* p1 = (uint64_t *)(p2[id3] &0xFFFFFFFFFFFFF000);
				int id4 = (vaddr_s >> 12 ) & 0x1FF;

				p1 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p1);

				p1[id4] = ((uint64_t)phy & 0xFFFFFFFFFFFFF000) | 7;
				return;
			}
		}
	}
}
/*
   uint64_t init_pages_for_process(uint64_t vaddr_s, uint64_t vaddr_e, uint64_t* pml4){
   uint64_t *pdpt,*pd,*pt;
   uint64_t idx1,idx2,idx3,idx4;

   pdpt = (uint64_t *)allocate_page_for_process();
   pd = (uint64_t *)allocate_page_for_process();
   pt = (uint64_t *)allocate_page_for_process();

   idx1 = (vaddr_s >> 39 ) & 0x1FF;
   idx2 = (vaddr_s >> 30 ) & 0x1FF;
   idx3 = (vaddr_s >> 21 ) & 0x1FF;
   idx4 = (vaddr_s >> 12 ) & 0x1FF;

   pml4[idx1] = (((uint64_t)pdpt-(uint64_t)0xffffffff80000000) & 0xFFFFFFFFFFFFF000) | 7;

   pml4[511] = (pml4e[511] & 0xFFFFFFFFFFFFF000) | 7;

   pdpt[idx2] = (((uint64_t)pd -(uint64_t)0xffffffff80000000) & 0xFFFFFFFFFFFFF000) | 7;
   pd[idx3] = (((uint64_t)pt-(uint64_t)0xffffffff80000000) & (0xFFFFFFFFFFFFF000)) | 7;

//      for(;phy_start<phy_end;phy_start+=4096){

for(;vaddr_s<vaddr_e;){
uint64_t phy = allocate_page();
idx1 = (vaddr_s >> 39 ) & 0x1FF;
idx2 = (vaddr_s >> 30 ) & 0x1FF;
idx3 = (vaddr_s >> 21 ) & 0x1FF;
idx4 = (vaddr_s >> 12 ) & 0x1FF;

if(idx4!=0){
pt[idx4] = (phy & 0xFFFFFFFFFFFFF000) | 7;
}
else{
pt = ((uint64_t*)allocate_page_for_process());
pd[idx3] = (((uint64_t)pt-(uint64_t)0xffffffff80000000) & (0xFFFFFFFFFFFFF000)) | 7;
pt[idx4] = (phy & 0xFFFFFFFFFFFFF000) | 7;
}
//map(vaddr_s,phy);
vaddr_s += 4096;
}	 
return (uint64_t)pml4;
}
 */


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


		//		map(viraddr,physbase);
	}
	k_cr3 = (uint64_t)pml4e;
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

	__asm__ volatile("movq %0,%%cr3"::"r"(k_cr3));	
//	k_cr3 = (uint64_t)pml4e;
}
void deletepagetables(uint64_t* pml4e){
	for(int i=0;i<511;i++){
		pml4e[i] = 0;
	}
}	
void copytables(task_struct* p, task_struct* c){
	uint64_t* p4 = (uint64_t *)(p->pml4e + 0xffffffff80000000);
	uint64_t* c4 =(uint64_t *) (c->pml4e + 0xffffffff80000000);
	c4[511] = p4[511];
	for(int i =0;i<511;i++){
		if(p4[i] & 1){
			kprintf("----******%d******------",i);
			uint64_t* c3 = (uint64_t *)allocate_page_for_process();
			c4[i] = ((uint64_t)((uint64_t)c3 -((uint64_t)0xffffffff80000000)) & 0xFFFFFFFFFFFFF000) | 7;
			uint64_t* p3 = (uint64_t *)(p4[i] & 0xFFFFFFFFFFFFF000);
			p3 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p3);
			for(int j=0;j<512;j++){
				if(p3[j] & 1){
					uint64_t* c2 = (uint64_t *)allocate_page_for_process();
					c3[j] = ((uint64_t)((uint64_t)c2 -((uint64_t)0xffffffff80000000)) & 0xFFFFFFFFFFFFF000) | 7;
					uint64_t* p2 = (uint64_t *)(p3[j] & 0xFFFFFFFFFFFFF000);
					p2 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p2);
					for(int k=0;k<512;k++){
						if(p2[k] & 1){
							uint64_t* c1 = (uint64_t *)allocate_page_for_process();
							c2[k] = ((uint64_t)((uint64_t)c1 -((uint64_t)0xffffffff80000000)) & 0xFFFFFFFFFFFFF000) | 7;
							uint64_t* p1 = (uint64_t *)(p2[k] & 0xFFFFFFFFFFFFF000);
							p1 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p1);
							for(int l=0;l<512;l++){
								if(p1[l]&1){
									p1[l] = (p1[l] & 0xFFFFFFFFFFFFFFFD) | (0x0000000000000200);//& 0xFFFFFFFFFFFFFFDF & 0xFFFFFFFFFFFFFFBF) ;
									c1[l] = p1[l];
									if(i == 32){
										kprintf("\n*****%p\n",p1[l]);
									}
								}
							}
						}	
					}
				}
			}
		}	
	}
}

void dealloc_pml4(uint64_t pm4){
    uint64_t* p4 = (uint64_t *)(pm4 + 0xffffffff80000000);
    for(int i=0;i<511;i++){
        if(p4[i]&1){
            uint64_t* p3 = (uint64_t *)(p4[i] & 0xFFFFFFFFFFFFF000);
            p3 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p3);
            for (int j = 0; j < 512; ++j) {
                if(p3[j] & 1){
                    uint64_t* p2 = (uint64_t *)(p3[j] & 0xFFFFFFFFFFFFF000);
                    p2 = (uint64_t *)((uint64_t)0xffffffff80000000 + (uint64_t)p2);
                    for (int k = 0; k < 512 ; ++k) {
                        if(p2[k]&1) {
                            uint64_t *p1 = (uint64_t * )(p2[k] & 0xFFFFFFFFFFFFF000);
                            p1 = (uint64_t*)((uint64_t) 0xffffffff80000000 + (uint64_t) p1);
                            for (int l = 0; l < 512; ++l) {
                                if(p1[l]&1){
                                    memset(&p1[l],0,4096);
                                    free(((uint64_t)p1[l] & 0xFFFFFFFFFFFFF000));
                                }
                            }

                        }

                    }
                }
            }
        }
    }
}
