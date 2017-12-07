#include<sys/defs.h>
#include<sys/process.h>
#include<sys/mem.h>
#include<sys/kprintf.h>
#include<sys/gdt.h>
#include<sys/string.h>
#include<sys/tarfs.h>
#include<sys/elf64.h>
#include <sys/terminal.h>

static task_struct* p;
static task_struct* new;
int newPID(){
	for(int i =0;i<MAX;i++){
		if(q[i].state == READY){
			return i;
		}
	}
	return -1;
}

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
void in(){
    while(1){
        wait();
    }
}
void idle(){

    while(1) {
        __asm__ volatile("sti");
        __asm__ volatile("hlt");
        __asm__ volatile("cli");
        yield();
    }

}
void init_p(){
    int pid = newPID();
    q[pid].pid = pid;
    strcpy(q[pid].name,"init");
    q[pid].state = RUNNING;
    q[pid].regs.rip = (uint64_t)&in;
    q[pid].regs.rsp = (uint64_t)(&(q[pid].kstack[511]));
    uint64_t pcr3;
    __asm__ volatile ("movq %%cr3,%0;" :"=r"(pcr3)::);
    q[pid].pml4e = pcr3;

    pid = newPID();
    q[pid].pid = pid;
    strcpy(q[pid].name,"idle");
    q[pid].state = RUNNING;
    q[pid].regs.rip = (uint64_t)&idle;
    q[pid].regs.rsp = (uint64_t)(&(q[pid].kstack[511]));
    __asm__ volatile ("movq %%cr3,%0;" :"=r"(pcr3)::);
    q[pid].pml4e = pcr3;
}

void ps()
{
	kprintf("ID    Name\n");
	for(int i=0; i<MAX; i++)
	{
        if(q[i].state == RUNNING || q[i].state == SLEEPING || q[i].state == WAIT || q[i].state == SUSPENDED) {
            kprintf("%d  ", q[i].pid);
            kprintf("%s  ", q[i].name);
            kprintf("\n");
        }
	}
}

void create_process(char* filename){
	//Load elf headers using the filename
	uint64_t f_a = get_file_address(filename) +512;
	if(f_a < 512){
		kprintf("No such file\n");
		return;
	}
    char a[50];
    strcpy(a,filename);
	int pid = newPID();
	task_struct* ts = (task_struct *) &q[pid];//(task_struct *) kmalloc(sizeof(struct task_struct));
	strcpy(ts->name,filename);
    strcpy(ts->curr_dir,"/");
    ts->ppid = 0;
	ts->pid = pid;
	ts->vm = NULL;

	ts->state = RUNNING;
	Elf64_Ehdr* eh = (Elf64_Ehdr*)(f_a); //512 - to offset tar info
	int no_ph = eh->e_phnum;
	uint64_t* pml4 = (uint64_t *)allocate_page_for_process();
	memset(pml4,0,4096);
	ts->pml4e =( uint64_t )((uint64_t)pml4 - (uint64_t)0xffffffff80000000);
	ts->regs.rip = eh->e_entry;
	for(int i=no_ph;i>0;i--){
		//               ep = ep + (i-1);
		Elf64_Phdr* ep = (Elf64_Phdr*)(f_a + (eh->e_phoff));
		ep = ep + (i-1);
		if(ep->p_type == 1){               

			vma* vm = (vma *)kmalloc(sizeof(struct vm_area_struct));
			vm->vm_start = ep->p_vaddr;
			vm->vm_end = ep->p_vaddr+ep->p_memsz;
            uint64_t k = vm->vm_start;
            if((((uint64_t)(ep->p_vaddr))% ((uint64_t)0x1000)) != 0){
                k = (uint64_t)((uint64_t)ep->p_vaddr & (uint64_t)0xFFFFFFFFFFFFF000);
            }
			if(ts->vm == NULL){
				vm->next = NULL;
				ts->vm = vm;
			}
			else{
				vm->next = ts->vm;
				ts->vm = vm;
			}
			for(;k<( vm->vm_end);k+=4096){
				uint64_t yy = allocate_page();
				init_pages_for_process(k,yy, pml4);
			}
			uint64_t pcr3;
			__asm__ __volatile__ ("movq %%cr3,%0;" :"=r"(pcr3)::);
			//                      *(pml4+511) = ((uint64_t)pdpte & 0xFFFFFFFFFFFFF000) | 7;
			uint64_t* pl =( uint64_t*)((uint64_t)pml4 - (uint64_t)0xffffffff80000000);

			__asm__ volatile ("movq %0, %%cr3;" :: "r"(pl));
			memcpy((void*)vm->vm_start,(void*)(eh + ep->p_offset), (uint64_t)(ep->p_filesz));
			memset((void*)(vm->vm_start + (uint64_t)(ep->p_filesz)), 0, (uint64_t)(ep->p_memsz) - (uint64_t)(ep->p_filesz));
			__asm__ volatile ("movq %0, %%cr3;" :: "r"(pcr3));
		}
	}

    vma* vm2 = (vma *)kmalloc(sizeof(struct vm_area_struct));
    vm2->vm_start = 0x4B0FFFFF0000;
    vm2->vm_end = 0x4B0FFFFF0000;
    vm2->next = ts->vm;
    ts->vm = vm2;


    uint64_t s_add = allocate_page();
	init_pages_for_process(0x100FFFFF0000,s_add,pml4);
	ts->ustack = (uint64_t*)0x100FFFFF0000;
	ts->rsp = (uint64_t *)((uint64_t)ts->ustack + (510 * 8));
	vma* vm = (vma *)kmalloc(sizeof(struct vm_area_struct));
	vm->vm_start = 0x100FFFFF0000;
	vm->vm_end = (uint64_t)0x100FFEFF0000;
	vm->next = ts->vm;
	ts->vm = vm;


	set_tss_rsp(&(ts->kstack[511]));

    uint64_t* pl =( uint64_t*)((uint64_t)pml4 - (uint64_t)0xffffffff80000000);
    __asm__ volatile ("movq %0, %%cr3;" :: "r"(pl));

    int len = strlen(a)+1;
    ts->rsp = ts->rsp - len;
    memcpy(ts->rsp,a,len);

    ts->rsp -= 1;
    *(ts->rsp) = 0;

    ts->rsp -= 1;
    *(ts->rsp) = 0;

    (ts->rsp)-=1;
    *(ts->rsp) = (uint64_t)((ts->rsp)+1);

    (ts->rsp)-=1;
    *(ts->rsp) = 0x1;

	r = ts;


	__asm__ volatile("pushq $0x23;pushq %0;pushf;pushq $0x2B;"::"r"(ts->rsp):"%rax","%rsp");
	__asm__ ("pushq %0"::"r"(ts->regs.rip):"memory");

	__asm__("iretq");
}

void copytask(task_struct* c){
	c->ppid = r->pid;

	c->pml4e = (uint64_t)((uint64_t)allocate_page_for_process() - (uint64_t)0xffffffff80000000);
    memset((uint64_t*)(c->pml4e+(0xffffffff80000000)),0,4096);
	strcpy(c->name,r->name);
    strcpy(c->curr_dir,r->curr_dir);

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
}
int fork(){

    int pid = newPID();
	new = (task_struct *) &q[pid];
	new->pid = pid;
	p = r;
	copytask(new);	

	uint64_t s_add ;
	new->ustack = (uint64_t*)0x100FFFFF0000;
	new->rsp = (uint64_t *)((uint64_t)new->ustack + (511*8));
	new->state = RUNNING;
	uint64_t pcr3;	
	__asm__ volatile ("movq %%cr3,%0;" :"=r"(pcr3)::);
	__asm__ volatile ("movq %0, %%cr3;" :: "r"(pcr3));	

	r->child_count+=1;
	memcpy(&(new->kstack[0]),&(r->kstack[0]),512*8);
    new->kstack[14] = 9999;
	__asm__ __volatile__(
            "movq 8(%%rsp),%%rax;movq %%rax, %0;"
			:"=g"(new->regs.rip)::"memory","%rax"
			);
	__asm__ __volatile__(
			"movq %%rsp, %0;"
			:"=g"(s_add)::"memory"
			);

    new->regs.rsp = (uint64_t) ((uint64_t)&(new->kstack[511]) -(uint64_t)((uint64_t)&(r->kstack[511]) - (uint64_t)s_add));
    return new->pid;
}

int execvpe(char* path, char *argv[],char* env[]){
	task_struct* ts = r;
    char file[50];
    int argc = 0, envl = 0;
    uint64_t f_a = 0 ;
    char args[10][80];
    char envs[40][80];
    if(path[0] == '.' && path[1] == '/'){
        strcpy(file, &(r->curr_dir[1]));
        strcat(file, path+2);
        uint64_t y = get_file_address(file)+512;
        if(y<512){
            return -1;
        }
        if( *((char *)y) == '!' && *((char *)y+1) == '#') {
            char ex[50];
            int q= 0;
            y+=2;
            while( (*(char *) y) != '\n'){
                if( (*(char *) y) != ' ') {
                    ex[q++] = *(char *) y;
                }
                y = y+1;
            }
            ex[q++]='\0';
            if(strlen(ex) == 0){
                strcpy(ex,"bin/sbush");
            }
            f_a = get_file_address(ex) + 512;
            strcpy(args[argc++], argv[0]);
            strcpy(args[argc++], path+2);
        }
        else {
            f_a = get_file_address("bin/sbush") + 512;
            strcpy(args[argc++], argv[0]);
            strcpy(args[argc++], path+2);
        }
    }
    else {
        strcpy(file, "bin/");
        strcat(file, path);
        strcpy(ts->name, file);


        f_a = get_file_address(file) + 512;
        if (f_a < 512) {
            return -1;
        }

        while (argv[argc]) {
            strcpy(args[argc], argv[argc]);
            argc++;
        }
    }
    while (env[envl]) {
        strcpy(envs[envl], env[envl]);
        argc++;
    }
    if (f_a < 512) {
        return -1;
    }
	Elf64_Ehdr* eh = (Elf64_Ehdr*)(f_a);
	int no_ph = eh->e_phnum;
	ts->regs.rip = eh->e_entry;
	uint64_t* pml4 = (uint64_t *)(ts->pml4e + 0xffffffff80000000);
    dealloc_pml4((ts->pml4e));

	for(int i=no_ph;i>0;i--){
		Elf64_Phdr* ep = (Elf64_Phdr*)(f_a + (eh->e_phoff));
		ep = ep + (i-1);
		if(ep->p_type == 1){               

			vma* vm = (vma *)kmalloc(sizeof(struct vm_area_struct));
			vm->vm_start = ep->p_vaddr;
			vm->vm_end = ep->p_vaddr+ep->p_memsz;
            uint64_t k = vm->vm_start;
            if((((uint64_t)(ep->p_vaddr))% ((uint64_t)0x1000)) != 0){
                k = (uint64_t)((uint64_t)ep->p_vaddr & (uint64_t)0xFFFFFFFFFFFFF000);
            }
			if(ts->vm == NULL){
				vm->next = NULL;
				ts->vm = vm;
			}
			else{
				vm->next = ts->vm;
				ts->vm = vm;
			}
			for(;k<( vm->vm_end);k+=4096){
				uint64_t yy = allocate_page();
				init_pages_for_process(k,yy, pml4);
			}

			uint64_t pcr3;
			__asm__ __volatile__ ("movq %%cr3,%0;" :"=r"(pcr3)::);
			uint64_t* pl =( uint64_t* )((uint64_t)pml4 - (uint64_t)0xffffffff80000000);
			__asm__ volatile ("movq %0, %%cr3;" :: "r"(pl));
			memcpy((void*)vm->vm_start,(void*)(eh + ep->p_offset), (uint64_t)(ep->p_filesz));
			memset((void*)(vm->vm_start + (uint64_t)(ep->p_filesz)), 0, (uint64_t)(ep->p_memsz) - (uint64_t)(ep->p_filesz));
			__asm__ volatile ("movq %0, %%cr3;" :: "r"(pcr3));
		}
	}

    vma* vm2 = (vma *)kmalloc(sizeof(struct vm_area_struct));
    vm2->vm_start = 0x4B0FFFFF0000;
    vm2->vm_end = 0x4B0FFFFF0000;
    vm2->next = ts->vm;
    ts->vm = vm2;


    uint64_t s_add = allocate_page();
	init_pages_for_process(0x100FFFFF0000,s_add,pml4);
	ts->ustack = (uint64_t*)0x100FFFFF0000;
	ts->rsp = (uint64_t *)((uint64_t)ts->ustack + (510 * 8));
	vma* vm = (vma *)kmalloc(sizeof(struct vm_area_struct));
	vm->vm_start = 0x100FFFFF0000;
	vm->vm_end = 0x100FFEFF0000;
	vm->next = ts->vm;
	ts->vm = vm;

    uint64_t* temp1[envl];
    for(int i=envl-1;i>=0;i--){
        int l = strlen(envs[i])+1;
        ts->rsp = ts->rsp-l;
        memcpy(ts->rsp,envs[i],l);
        temp1[i] = ts->rsp;
    }

	uint64_t* temp[argc];
	for(int i=argc-1;i>=0;i--){
		int l = strlen(args[i])+1;
		ts->rsp = ts->rsp-l;
		memcpy(ts->rsp,args[i],l);
		temp[i] = ts->rsp;
	}

    ts->rsp -= 1;
    *(ts->rsp) = 0;
    for(int i=envl-1;i>=0;i--){
        (ts->rsp)-=1;
        *(ts->rsp) = (uint64_t)temp1[i];
    }

    ts->rsp -= 1;
    *(ts->rsp) = 0;

    for(int i=argc-1;i>=0;i--){
		(ts->rsp)-=1;
		*(ts->rsp) = (uint64_t)temp[i];
	}
	(ts->rsp)-=1;
	*(ts->rsp) = argc;
	set_tss_rsp(&(ts->kstack[511]));
	__asm__ volatile("\
	push $0x23;\
	push %0;\
	pushf;\
	push $0x2B;\
	push %1"::"g"(ts->rsp),"g"(ts->regs.rip):"memory");
	__asm__ volatile("iretq;");
	return 1;
}

void exit(){
    r->state = ZOMBIE;
    for (int i = 0; i < MAX; ++i) {
        if(q[i].ppid == r->pid){
            q[i].ppid = 0;
        }
    }
    if(q[r->ppid].state ==  WAIT){
        q[r->ppid].state =  RUNNING;
    }
    yield();
}
void removeProcess(int i){
    vma* vm = q[i].vm;
    while(vm){
        uint64_t k = (uint64_t)vm;
        vm = vm->next;
        free(k-0xffffffff80000000);
    }
    dealloc_pml4(q[i].pml4e);
    free(q[i].pml4e);
}
int wait(){
    while(1) {
        for (int i = 0; i < MAX; ++i) {
            if ((q[i].ppid == r->pid) && (q[i].state == ZOMBIE)) {
                removeProcess(i);
                q[i].state = READY;
                return i;
            }
        }
        r->state = WAIT;
        yield();
        for (int i = 0; i < MAX; ++i) {
            if ((q[i].ppid == r->pid) && (q[i].state == ZOMBIE)) {
                removeProcess(i);
                q[i].state = READY;
                return i;
            }
        }
    }
}
int kill(int pid){
    q[pid].state = ZOMBIE;
    for (int i = 0; i < MAX; ++i) {
        if(q[i].ppid == pid){
            q[i].ppid = 0;
        }
    }
    if(q[q[pid].ppid].state == WAIT){
        q[q[pid].ppid].state = RUNNING;
    }
    return 1;
}
int waitpid(int pid){
    int i = pid;
    if((q[i].ppid == r->pid) && (q[i].state == ZOMBIE)){
        removeProcess(i);
        q[i].state = READY;
        return i;
    }
    r->state = WAIT;
    while(1){
        yield();
        if((q[i].ppid == r->pid) && (q[i].state == ZOMBIE)){
            removeProcess(i);
            q[i].state = READY;
            return i;
        }
    }
}

pid_t getpid(void){
	return r->pid;
}

pid_t getppid(void){
	return r->ppid;
}

unsigned int sleep(unsigned int seconds){
    if(seconds <= 0){
        return 0;
    }
    r->time = seconds;
    r->state = SUSPENDED;
    yield();
    return 0;
}
int chdir(char* path){
    if(strcmp("../",path) == 0){
        char k[100];
        strcpy(k,path);
        setTruePath(k);
        if(strcmp(k,"")==0){
            strcpy(r->curr_dir,"/");
            return 0;
        }
    }
    if((isValidDirectory(path)) > -1){
        char k[100];
        strcpy(k,path);
        setTruePath(k);
        char l[100];
        l[0] = '/';
        l[1] = '\0';
        strcat(l,k);
        strcpy(r->curr_dir,l);
        return 0;
    }
    return  -1;
}
void getcwd(char *buf, int size){

    strcpy(buf,r->curr_dir);
    int l = strlen(buf);
    if(l == 1){
        return;
    }
    else{
        buf[l-1] = '\0';
    }
}
void* malloc(int no_of_bytes){
    vma* vm = r->vm->next;

    uint64_t ret = vm->vm_end;
    for(int i =0;i<((no_of_bytes/4096))+1;i++){
        uint64_t s_add = allocate_page();
        init_pages_for_process(vm->vm_end,s_add,(uint64_t *)(r->pml4e+0xffffffff80000000));
        vm->vm_end = vm->vm_end + 4096;
    }
    return (uint64_t*)ret;
}