#include<sys/defs.h>
#include<sys/process.h>
#include<sys/mem.h>
#include<sys/kprintf.h>
#include<sys/gdt.h>
#include<syscall.h>
#include<sys/string.h>
#include<sys/tarfs.h>
#include<sys/elf64.h>
static task_struct *curr_task;
static task_struct main_task;
static task_struct next_task;
//static task_struct u_p;
//int len = 99;
int newPID(){
	for(int i =0;i<MAX;i++){
		if(pid[i] == 0){
			pid[i] = 1;
			return i;
		}
	}
	return -1;
}
/*void strcpy(char *string2, char *string1){

        while(*string1)
        {   
                *string2=*string1;
                string1++;
                string2++;
        }   
        *string2='\0';
}*/

void *memcpy(void *dst,void *src, uint64_t count)
{
        char *dest= dst;
        char *source= src;
        while(count--)
        {
                *dest=*source;
                dest++;
                source++;
        }
	return 0;
}
int user_process_test(){
	//This should throw isr14 hopefully if ring0 to ring 3 works correctly.
//	__asm__ volatile("int $0x80;");
//	_syscall3(int,write,int,stdout,char*,s,int,len);;
//	char k = 'a';
//	char *s = "Hello\n";

	int len ;
	len = 98;	
	_syscall3(int,write,int,stdout,int*,&len,int,len);
	
//	kprintf("------------------------- %d\n",len);

//	_syscall3(int,write,int,stdout,int*,&k,int,1);	
//	kprintf("%d",i);

//	__asm__ volatile("retq");
//	return 0;
}
void switchtor3(){
	uint64_t fad =  (uint64_t)&user_process_test;
	uint64_t *rsp;
	//create_task(&u_p,(uint64_t)fad,main_task.regs.eflags,(uint64_t)main_task.regs.cr3);		
	__asm__ volatile("movq %%rsp,%0;":"=r"(rsp)::"memory");
	__asm__ volatile("movq %%rsp,%%rax;pushq $0x23;pushq %%rax;pushf;pushq $0x2B;":::"%rax","%rsp");
	__asm__ ("pushq %0"::"r"(fad):"memory");
	set_tss_rsp(rsp);
	__asm__ volatile("iretq");
	kprintf("Fam WHAAAAAAAAAAAT");
}
static void next_main() {
	kprintf("Test Context Switching\n");
	yield();
}

void init_task() {
	__asm__ volatile("movq %%cr3, %%rax; movq %%rax, %0;":"=m"(main_task.regs.cr3)::"%rax");
	__asm__ volatile("pushq %%rax; movq (%%rsp), %%rax; movq %%rax, %0; popq %%rax;":"=m"(main_task.regs.rflags)::"%rax");

	create_task(&next_task,(uint64_t) &next_main, main_task.regs.rflags, (uint64_t)main_task.regs.cr3);

	main_task.next = &next_task;
	next_task.next = &main_task;
	curr_task = &main_task;
}
void create_process(char* filename){
        //Load elf headers using the filename
        uint64_t f_a = get_file_address(filename) +512;
        if(f_a < 0){
                kprintf("No such file\n");
                return;
        }
        
        task_struct* ts = (task_struct *) kmalloc(sizeof(struct task_struct));
        strcpy(ts->name,filename);
        ts->ppid = -1;
        ts->pid = newPID();
        ts->vm = NULL;

        Elf64_Ehdr* eh = (Elf64_Ehdr*)(f_a); //512 - to offset tar info
        int no_ph = eh->e_phnum;
        uint64_t* pml4 = (uint64_t *)allocate_page_for_process();
	memset(pml4,0,4096);
        ts->pml4e =( uint64_t )((uint64_t)pml4 - (uint64_t)0xffffffff80000000); 
       	ts->regs.rip = eh->e_entry;
//	pml4[511] = pml4e;
//	uint64_t s_add = allocate_page_for_process();

  //      init_pages_for_process(0x300000,(s_add - 0xffffffff80000000),pml4);
    
//        ts->ustack = (uint64_t*)0x300000;
//        ts->rsp = (uint64_t *)((uint64_t)ts->ustack + (511 * 8));




//	Elf64_Phdr* ep = (Elf64_Phdr*)(f_a + (eh->e_phoff));
        for(int i=no_ph;i>0;i--){
 //               ep = ep + (i-1);
		 Elf64_Phdr* ep = (Elf64_Phdr*)(f_a + (eh->e_phoff));
		ep = ep + (i-1);
		if(ep->p_type == 1){               
                        
                        vma* vm = (vma *)kmalloc(sizeof(struct vm_area_struct));
                        vm->vm_start = ep->p_vaddr;
                        vm->vm_end = ep->p_vaddr+ep->p_memsz;
                        if(ts->vm == NULL){
                                vm->next = NULL;
                                ts->vm = vm;
                        }
                        else{
                                vm->next = ts->vm;
                                ts->vm = vm;
                        }
                        for(uint64_t k = vm->vm_start;k<( vm->vm_end);k+=4096){ 
				uint64_t yy = allocate_page();
				init_pages_for_process(k,yy, pml4);
                        }

			uint64_t pcr3;
                        __asm__ __volatile__ ("movq %%cr3,%0;" :"=r"(pcr3)::);
//                      *(pml4+511) = ((uint64_t)pdpte & 0xFFFFFFFFFFFFF000) | 7;
			uint64_t* pl =( uint64_t* )((uint64_t)pml4 - (uint64_t)0xffffffff80000000);			
			
			__asm__ volatile ("movq %0, %%cr3;" :: "r"(pl));
                        memcpy((void*)vm->vm_start,(void*)(eh + ep->p_offset), (uint64_t)(ep->p_filesz));
			memset((void*)vm->vm_start + (uint64_t)(ep->p_filesz), 0, (uint64_t)(ep->p_memsz) - (uint64_t)(ep->p_filesz));
                        __asm__ volatile ("movq %0, %%cr3;" :: "r"(pcr3));
                }
               // ep = ep+1;
        }
	
	
	uint64_t s_add = allocate_page();
	init_pages_for_process(0x100FFFFF0000,s_add,pml4);
	ts->ustack = (uint64_t*)0x100FFFFF0000;
	ts->rsp = (uint64_t *)((uint64_t)ts->ustack + (510 * 8));

//	ts->ustack = (uint64_t*) allocate_page_for_process();
	
//	ts->rsp = (uint64_t *)((uint64_t)ts->ustack + (511*8));
	
	
	set_tss_rsp(&(ts->kstack[511]));
	
	r = ts;
	addToQ(*ts);
	 uint64_t* pl =( uint64_t* )((uint64_t)pml4 - (uint64_t)0xffffffff80000000);
	__asm__ volatile ("movq %0, %%cr3;" :: "r"(pl));

	__asm__ volatile("pushq $0x23;pushq %0;pushf;pushq $0x2B;"::"r"(ts->rsp):"%rax","%rsp");
	__asm__ ("pushq %0"::"r"(ts->regs.rip):"memory");
    
	__asm__("iretq");
        //Load elf sections into memory.
//        kprintf("Atleast this works %d,%p",msize,svaddr);
}

void create_task(task_struct *task, uint64_t main, uint64_t flags, uint64_t pagedir) {
	task->regs.rax = 0;
	task->regs.rbx = 0;
	task->regs.rcx = 0;
	task->regs.rdx = 0;
	task->regs.rsi = 0;
	task->regs.rdi = 0;
	task->regs.rflags = flags;
	task->regs.rip = (uint64_t) main;
	task->regs.cr3 = (uint64_t) pagedir;
	task->regs.rsp = (uint64_t)allocate_page_for_process() + 0xFF8; // Not implemented here

	for(int i = 0;i<9;i++){
		uint64_t* temp = (uint64_t*)(task->regs.rsp);
		if(i == 0){
			*temp = main;
		}
		else{
			*temp = i;
		}
		task->regs.rsp = (uint64_t)--temp;	
	}
	task->next = 0;
}
void copytask(task_struct* c){
	
	c->pid = newPID();
	c->ppid = r->pid;
		
	c->pml4e = (uint64_t)((uint64_t)allocate_page_for_process() - (uint64_t)0xffffffff80000000);

	strcpy(c->name,r->name);
	
	copytables(r,c);
	
	vma* a = r->vm;
	vma* p = NULL;
	c->vm = NULL;
	while(a!=NULL){	
		vma* new = (vma *)kmalloc(sizeof(struct vm_area_struct));
		memcpy(new,a,sizeof(struct vm_area_struct));
		if(p == NULL){
			p = new;
			c->vm = p;	
		}
		else{
			p->next = new;
			p = new;
		}
		a = a->next;
	}

			
	memcpy(&(c->kstack[0]),&(r->kstack[0]),512*8);


//	memcpy(c->ustack,r->ustack,512*8);	
	
}
int fork(){
	task_struct* new = (task_struct *) kmalloc(sizeof(struct task_struct));
//	new->ustack = (uint64_t*) allocate_page();    
	
  //      new->rsp = (uint64_t *)((uint64_t)new->ustack + (511*8));
	
	copytask(new);	
//	new->ustack = (uint64_t*) allocate_page(); 
	uint64_t s_add = allocate_page();
	init_pages_for_process(0xFFFFF0000,s_add,(uint64_t *)(new->pml4e + 0xffffffff80000000));
	new->ustack = (uint64_t*)0xFFFFF0000;
	new->rsp = (uint64_t *)((uint64_t)new->ustack + (511*8));
	
	uint64_t pcr3;
	__asm__ volatile ("movq %%cr3,%0;" :"=r"(pcr3)::);
	
	r->child_count+=1;
	r->child = new;
	r->next = new;	
	addToQ(*new);

	__asm__ volatile ("movq %0,%%cr3;" ::"r"(r->pml4e):);

	char temp[4096];
	memcpy(&temp[0],r->ustack,512*8);
	__asm__ volatile ("movq %0,%%cr3;" ::"r"(new->pml4e):);
	memcpy(new->ustack,&temp[0],512*8);

	__asm__ volatile ("movq %0,%%cr3;" ::"r"(r->pml4e):);
	__asm__ __volatile__(
                "movq $2f, %0;"
		"2:\t"
                :"=g"(new->regs.rip)::
        );

	__asm__ __volatile__(
                "movq %%rsp, %0;"
                :"=r"(new->rsp)::
	);
	



	//Flush TLB
	__asm__ volatile ("movq %0, %%cr3;" :: "r"(pcr3));
	
	if(r->ppid == -1){
		return new->pid;
	}
	else{
		return 0;
	}
}/*
void yield() {
	task_struct *last = r;
	curr_task = curr_task->next;
	__asm__ volatile("pushq %%rax ;pushq %%rcx ;pushq %%rdx ;pushq %%rsi ;pushq %%rdi ;pushq %%r8 ;pushq %%r9 ;pushq %%r10;pushq %%r11;":::);
	__asm__ volatile("movq %%rsp, %0;":"=g"((last->rsp))::"memory");
	__asm__ volatile("movq %0,%%rsp;"::"r"(curr_task->rsp):"memory");
	__asm__ volatile("popq %%r11; popq %%r10;popq %%r9 ;popq %%r8 ;popq %%rdi ;popq %%rsi ;popq %%rdx ;popq %%rcx ;popq %%rax":::);
	__asm__ volatile("movq %%rsp, %0;":"=g"((curr_task->rsp))::"memory");
	__asm__ volatile("pushq (%0);"::"r"(curr_task->rsp):"memory");
	__asm__ volatile("retq");

}*/
