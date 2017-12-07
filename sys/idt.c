#include <sys/idt.h>
#include <sys/kprintf.h>
#include <sys/mem.h>
#include <sys/process.h>
#include <sys/file.h>
#include <sys/terminal.h>
uint16_t PIT_reload_value = 1193;
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
//uint64_t ret= 0;
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
void init_timer(){
    uint8_t lobyte = (uint8_t)(PIT_reload_value & 0xFF);
    uint8_t hibyte = (uint8_t)((PIT_reload_value >> 8) & 0xFF);
    outportb(0x40, lobyte);
    outportb(0x40, hibyte);
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
  //  init_timer();
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
	while(1);
	outportb(0x20,0x20);

}
void isr14(){

	uint64_t bb;
    int flag = 0;
	__asm__ volatile("movq %%cr2,%0;":"=g"(bb)::);
    vma* vm = r->vm;
    if(((vm->vm_start + 4096) > bb) && (vm->vm_end < bb)){
        flag =1;
    }
    if(flag == 0) {
        while (vm != NULL) {
            if ((vm->vm_start < bb) && (vm->vm_end > bb)) {
                flag = 1;
                break;
            }
            vm = vm->next;
        }
    }
    if(flag == 0){
       // kprintf("New allocated stack at %p - range %p,%p \n",bb,r->vm->vm_start+4096,r->vm->vm_end);

        kprintf("Segmentation Fault: Address:%p \n",bb);
            exit();
            while(1);
    }
    uint64_t* k = getPTE(bb);
    if( k[(bb>>12)&0x1FF] & 0x0000000000000200){     //COW
        uint64_t k1;
        __asm__ volatile("movq %%cr3,%0;":"=g"(k1)::);
        uint64_t  add = (k[(bb>>12)&0x1FF] & 0xFFFFFFFFFFFFF000);
        int i = getrefcount(add);
        if(i == 2){
            uint64_t p_n = allocate_page();
            memcpy((void*)(0xffffffff80000000 + p_n),(void *)(bb&0xFFFFFFFFFFFFF000),4096);
            switchtokern();
            init_pages_for_process((bb&0xFFFFFFFFFFFFF000),p_n,(uint64_t *)(r->pml4e + 0xffffffff80000000));
            free(add);
        } else if(i == 1){
            k[(bb>>12)&0x1FF] = (k[(bb>>12)&0x1FF] | 0x2) & 0xFFFFFFFFFFFFFDFF;
        } else{
            kprintf("Should never be here\n");
            while(1);
        }
        __asm__ volatile("movq %0,%%cr3;"::"r"(k1):);
	}
	else if( (r->vm->vm_start > bb)  && (r->vm->vm_end < bb)){   //Auto Growing stack
        uint64_t k;
		__asm__ volatile("movq %%cr3,%0;":"=g"(k)::);
		//uint64_t n_s = r->vm->vm_start - 4096;
		uint64_t p_n = allocate_page();
		switchtokern();
		init_pages_for_process((bb&0xFFFFFFFFFFFFF000),p_n,(uint64_t *)(r->pml4e + 0xffffffff80000000));
	//	r->vm->vm_start = n_s;
		 __asm__ volatile("movq %0,%%cr3;"::"r"(k):);
//		while(1);
	}
    else{
        kprintf("Segmentation Fault: Address:%p \n",bb);
        exit();
        while(1);
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
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rbp, rdi, rsi, rdx, rcx, rbx;
}registers_t;

uint64_t isr128(){
	uint64_t cval;
	 uint64_t as,ret = 0;
	__asm__ volatile("movq %%rax,%0;":"=g"(cval)::"memory","r15","rax");
	__asm__ volatile("movq %%rdi,%0;":"=g"(as)::"memory","rdi","rax");
	registers_t *y = (registers_t *)as;
    	if(cval == 0 && y->rbx == 0){
        	read_input((char *)y->rcx);
    	}
    	else if(cval == 1 && y->rbx == 1){ //This is a write syscall to stdout
		kprintf("%s",y->rcx);
	}
	else if(cval == 57){
		ret = (uint64_t)fork();
        if((uint64_t)r->kstack[14] == 9999){
            ret = 0;
            r->kstack[14] = 0;
            __asm__ volatile("popq %%rax":::"%rax");
            __asm__ volatile("popq %%rax":::"%rax");
        }
	}
	else if(cval == 59){
		ret = execvpe((char *)y->rbx,(char **)y->rcx,(char **) y->rdx);
	}
	else if(cval == 0){
		ret = read_tarfs((int) y->rbx, (char*) y->rcx, (int) y->rdx);			
	}
	else if(cval == 2){
		ret = open_tarfs((char*) y->rbx, (int) y->rcx);
	}else if(cval == 3) {
            ret = close_tarfs((int)y->rbx);
    }
   else if(cval == 16){
        ret =  open_dir((char *)y->rbx);
    }
   else if(cval == 78){
		ret = readdir_tarfs((int) y->rbx, (char *) y->rcx);
   }
    	else if(cval == 60){
      	 	exit();
    	}
    	else if(cval == 247){
        	ret = (uint64_t)waitpid((int)y->rbx);
    	}
	else if(cval == 39){
		ret = getpid();
	}
	else if(cval == 110){
		ret = getppid();
	}
    else if(cval == 7){
            clrscr();
    }
    else if(cval == 62){
       ret = kill((int)y->rbx);
    }
    else if(cval == 35){
            ret = sleep((int)y->rbx);
    }
    else if(cval == 79){
            getcwd((char*)y->rbx,(int)y->rcx);
    }
    else if(cval == 80){
            ret = chdir((char*)y->rbx);
    }
	else if(cval == 299){
		ps();
	}else if(cval == 9){
            ret = (uint64_t) malloc((int)y->rbx);
        }
	// yield();
	outportb(0x20,0x20);
	return ret;
}

void outportb(uint16_t port,uint8_t data){
	__asm__ volatile("outb %1,%0"
			:
			:"dN"(port),"a"(data));
}
