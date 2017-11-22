#include<sys/defs.h>
#include<sys/process.h>
#include<sys/mem.h>
#include<sys/kprintf.h>
#include<sys/gdt.h>
#include<syscall.h>
#include<string.h>
#include<sys/tarfs.h>
#include<sys/elf64.h>
static task_struct *curr_task;
static task_struct main_task;
static task_struct next_task;
//static task_struct u_p;
//int len = 99;
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
	uint64_t f_a = get_file_address(filename);
	if(f_a < 0){
		kprintf("No such file\n");
		return;
	}
	Elf64_Ehdr* eh = (Elf64_Ehdr*)(f_a); //512 - to offset tar info
	Elf64_Phdr* ep = (Elf64_Phdr*)(eh + (eh->e_phoff));
	
	int msize = ep->p_memsz;
	int svaddr = ep->p_vaddr;
	
	kprintf("Atleast this works %d,%p",msize,svaddr);
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

void yield() {
	task_struct *last = curr_task;
	curr_task = curr_task->next;
	__asm__ volatile("pushq %%rax ;pushq %%rcx ;pushq %%rdx ;pushq %%rsi ;pushq %%rdi ;pushq %%r8 ;pushq %%r9 ;pushq %%r10;pushq %%r11;":::);
	__asm__ volatile("movq %%rsp, %0;":"=g"((last->regs.rsp))::"memory");
	__asm__ volatile("movq %0,%%rsp;"::"r"(curr_task->regs.rsp):"memory");
	__asm__ volatile("popq %%r11; popq %%r10;popq %%r9 ;popq %%r8 ;popq %%rdi ;popq %%rsi ;popq %%rdx ;popq %%rcx ;popq %%rax":::);
	__asm__ volatile("movq %%rsp, %0;":"=g"((curr_task->regs.rsp))::"memory");
	__asm__ volatile("pushq (%0);"::"r"(curr_task->regs.rsp):"memory");
	__asm__ volatile("retq");
}
