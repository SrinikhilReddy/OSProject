#include <sys/defs.h>
#include <sys/gdt.h>
#include <sys/kprintf.h>
#include <sys/tarfs.h>
#include <sys/ahci.h>
#include <sys/idt.h>
#include<sys/process.h>
#include <sys/mem.h>
#include <sys/tarfs.h>
#include <sys/file.h>
#include <dirent.h>
#include <stdlib.h>
#define INITIAL_STACK_SIZE 4096
uint8_t initial_stack[INITIAL_STACK_SIZE]__attribute__((aligned(16)));
uint32_t* loader_stack;
extern char kernmem, physbase;

uint64_t max;
void start(uint32_t *modulep, void *physbase, void *physfree)
{
  struct smap_t* smap;
  while(modulep[0] != 0x9001) modulep += modulep[1]+2;
  for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
    if (smap->type == 1  && smap->length != 0) {
      	mem_map(smap,(uint64_t)physbase,(uint64_t)physfree);
	kprintf("Available Physical Memory [%p-%p]\n", smap->base, smap->base + smap->length);
	max = smap->base+smap->length; 
    }
  }

  init_ia32e_paging((uint64_t)0, max);
	 init_proc();
	init_tarfs();
	r = &q[0];
	r->state = RUNNING;
	char buf[100];
	int fd = open_tarfs("bin/", 0);
	int fd1 = readdir_tarfs(fd,&buf[0]);
	kprintf("%d: %s", fd1, buf);
	fd1 = readdir_tarfs(fd,&buf[0]);
	kprintf("%d,:,%s", fd1, buf);
	while(1);
	/*fd1 = readdir_tarfs(fd,&buf[0]); 
	kprintf("%d,:,%s", fd1, buf);
	fd1 = readdir_tarfs(fd,&buf[0]);
	kprintf("%d,:,%s", fd1, buf);*/
//  create_process("bin/ls");
  __asm__ volatile("sti");
//  checkAllBuses();
//  kprintf("physfree %p\n", (uint64_t)physfree);
//  kprintf("physbase %p\n", (uint64_t)physbase);
 // kprintf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);

//  init_task();
//  kprintf("Switching to next task\n");
 // yield();
//  kprintf("Returned to main task\n");
//  switchtor3(); 
//   create_process("bin/sbush");  
//  switchtor3();
  //  init_p();
  // create_process("bin/sbush");  
  while(1);
}

void boot(void)
{
  // note: function changes rsp, local stack variables can't be practically used
// register char /* *temp1, */ *temp2;
// for(temp2 = (char*)0xb8001; temp2 < (char*)0xb8000+160*25; temp2 += 2) *temp2 = 7 /* white */;
  __asm__ volatile (
    "cli;"
    "movq %%rsp, %0;"
    "movq %1, %%rsp;"
    :"=g"(loader_stack)
    :"r"(&initial_stack[INITIAL_STACK_SIZE])
  );
 
 init_gdt();
  start(
    (uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
    (uint64_t*)&physbase,
    (uint64_t*)(uint64_t)loader_stack[4]
  );
/*  for(
    temp1 = "!!!!! start() returned !!!!!", temp2 = (char*)0xb8000;
    *temp1;
    temp1 += 1, temp2 += 2
  ) *temp2 = *temp1;*/
  while(1) __asm__ volatile ("hlt");
}
