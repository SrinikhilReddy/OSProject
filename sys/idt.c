#include <sys/idt.h>
#include <sys/kprintf.h>
#include <sys/mem.h>
#include <sys/process.h>

static struct idt idt_table[256];
static struct idt_ptr pr;

 void isr_0();
 void isr_1();
 void isr_2();
 void isr_3();
 void isr_4();
 void isr_5();
 void isr_6();
 void isr_7();
 void isr_8();
 void isr_9();
 void isr_10();
 void isr_11();
 void isr_12();
 void isr_13();
 void isr_14();
 void isr_15();
 void isr_16();
 void isr_17();
 void isr_18();
 void isr_19();
 void isr_20();
 void isr_21();
 void isr_22();
 void isr_23();
 void isr_24();
 void isr_25();
 void isr_26();
 void isr_27();
 void isr_28();
 void isr_29();
 void isr_30();
 void isr_31();
 void isr_128();
extern void timer();
extern  void kb1();
// void kb();
uint64_t ret= 0;
void set_value(uint16_t intNum,uint64_t handler)
{
	idt_table[intNum].selector  = 0x08;
	idt_table[intNum].lower_offset = handler & 0xFFFF ;
	idt_table[intNum].mid_offset = (handler >> 16) & 0xFFFF;
	idt_table[intNum].high_offset = (handler >> 32) & 0xFFFFFFFF;
	idt_table[intNum].zero_1 = 0;
	if(intNum == 128){
		idt_table[intNum].type = 0xEE;
	}
	else{
	idt_table[intNum].type = 0x8E;
	}
	idt_table[intNum].zero_2 = 0;
}

static inline uint8_t inportb(uint64_t port)
{
        uint8_t r;
        __asm__ volatile( 
                        "inb %1, %0"
                        : "=a"(r)
                        : "Nd"(port)
                        );  
        return r;
}
void init_idt(){
//	set_value(0,(uint64_t)&isr_0);
	set_value(0 ,(uint64_t)&isr_0 );
set_value(1 ,(uint64_t)&isr_1);
set_value(2 ,(uint64_t)&isr_2);
set_value(3 ,(uint64_t)&isr_3);
set_value(4 ,(uint64_t)&isr_4);
set_value(5 ,(uint64_t)&isr_5);
set_value(6 ,(uint64_t)&isr_6);
set_value(7 ,(uint64_t)&isr_7);
set_value(8 ,(uint64_t)&isr_8);
set_value(9 ,(uint64_t)&isr_9);
set_value(10,(uint64_t)&isr_10) ;
set_value(11,(uint64_t)&isr_11) ;
set_value(12,(uint64_t)&isr_12) ;
set_value(13,(uint64_t)&isr_13) ;
set_value(14,(uint64_t)&isr_14) ;
set_value(15,(uint64_t)&isr_15) ;
set_value(16,(uint64_t)&isr_16) ;
set_value(17,(uint64_t)&isr_17) ;
set_value(18,(uint64_t)&isr_18) ;
set_value(19,(uint64_t)&isr_19) ;
set_value(20,(uint64_t)&isr_20) ;
set_value(21,(uint64_t)&isr_21) ;
set_value(22,(uint64_t)&isr_22) ;
set_value(23,(uint64_t)&isr_23) ;
set_value(24,(uint64_t)&isr_24) ;
set_value(25,(uint64_t)&isr_25) ;
set_value(26,(uint64_t)&isr_26) ;
set_value(27,(uint64_t)&isr_27) ;
set_value(28,(uint64_t)&isr_28) ;
set_value(29,(uint64_t)&isr_29) ;
set_value(30,(uint64_t)&isr_30) ;
set_value(31,(uint64_t)&isr_31) ;	
set_value(128,(uint64_t)&isr_128) ;
	set_value(32,(uint64_t)&timer);
	pr.size = (sizeof(struct idt) * 256) - 1 ;
	pr.base = (uint64_t)idt_table;
	set_value(33,(uint64_t)&kb1);
//	for(int i = 34;i<255;i++){
//		set_value(i,(uint64_t)&isr);
//	}
//	set_value(0x20,(uint64_t)&timer);
	__asm__("lidt (%0)"::"r"(&pr));
}

void isr0(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr1(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr2(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr3(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr4(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr5(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr6(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr7(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr8(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr9(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr10(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr11(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr12(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr13(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr14(){
	uint64_t bb;
	__asm__ volatile("movq %%cr2,%0;":"=g"(bb)::);
	if( bb & 0x0000000000000200){
		uint64_t k ;//= getPTE(bb); 
		__asm__ volatile("movq %%cr3,%0;":"=g"(k)::);
		//TODO: Free the old page
		uint64_t p_n = allocate_page();
		memcpy((uint64_t*)(0xffffffff80000000 + p_n),(uint64_t *)(bb&0xFFFFFFFFFFFFF000),4096);
		switchtokern();
		init_pages_for_process(bb,p_n,(uint64_t *)(r->pml4e + 0xffffffff80000000));
		__asm__ volatile("movq %0,%%cr3;"::"r"(k):);
		outportb(0x20,0x20);
	}
	else if( ((r->vm->vm_start) > bb) && ((r->vm->vm_start - 4096) < bb)){
		uint64_t k ;
		__asm__ volatile("movq %%cr3,%0;":"=g"(k)::);
		uint64_t n_s = r->vm->vm_start - 4096;
		uint64_t p_n = allocate_page();
		switchtokern();
		init_pages_for_process(n_s,p_n,(uint64_t *)(r->pml4e + 0xffffffff80000000));	
		 __asm__ volatile("movq %0,%%cr3;"::"r"(k):);
		outportb(0x20,0x20);
	}
	else{
		kprintf("Page Fault!!!!!!");
		kprintf("%p",bb);
		while(1);
		outportb(0x20,0x20);
	}

}
void isr15(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr16(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr17(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr18(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr19(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr20(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr21(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr22(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr23(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr24(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr25(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr26(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr27(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr28(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr29(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr30(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
void isr31(){
	kprintf("This is an exception");
	outportb(0x20,0x20);
}
typedef struct registers_t{
///	uint64_t rbp,rdi,rsi,rdx,rcx,rbx,rax;
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rbp, rdi, rsi, rdx, rcx, rbx, rax;
}registers_t;
uint64_t isr128(){
        kprintf("Interrupt 80 raised!!!!");
//	yield();
	uint64_t as;//ret = 0;

	__asm__ volatile("movq %%r15,%0;":"=g"(as)::"memory","r15");
        registers_t *y = (registers_t *)as;
	
	kprintf("_________________%d",y->rax);
	if(y->rax == 1 && y->rbx == 1){ //This is a write syscall to stdout
		int* i = (int *)(y->rcx);
		kprintf("\n%d\n",(*i));
	}
	else if(y->rax == 57){
			
		kprintf("\n ------ FORK ------- \n");
		ret = (uint64_t)fork();
//		yield();
	}
	else if(y->rax == 59){
		kprintf("/////////////////////////\n**********************************\n");
		execvpe((char *)y->rbx,(char **)y->rcx);
		yield();
	}
//	if(y->rax == 22){
//		yield();
//	}
	outportb(0x20,0x20);
	return ret;
}
void outportb(uint16_t port,uint8_t data){
	__asm__ volatile("outb %1,%0"
			:
			:"dN"(port),"a"(data));
}

