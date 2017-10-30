#include<sys/defs.h>
#include<stdint.h>
typedef struct SMAP_entry {
 
	uint32_t BaseL; // base address uint64_t
	uint32_t BaseH;
	uint32_t LengthL; // length uint64_t
	uint32_t LengthH;
	uint32_t Type; // entry Type
	uint32_t ACPI; // extended
 
}__attribute__((packed)) SMAP_entry_t;
 
// load memory map to buffer - note: regparm(3) avoids stack issues with gcc in real mode
int __attribute__((noinline)) __attribute__((regparm(3))) detectMemory(SMAP_entry_t* buffer, int maxentries)
{
	uint32_t contID = 0;
	int entries = 0, signature, bytes;
	do 
	{
		__asm__ __volatile__ ("int  $0x15" 
				: "=a"(signature), "=c"(bytes), "=b"(contID)
				: "a"(0xE820), "b"(contID), "c"(24), "d"(0x534D4150), "D"(buffer));
		if (signature != 0x534D4150) 
			return -1; // error
		if (bytes > 20 && (buffer->ACPI & 0x0001) == 0)
		{
			// ignore this entry
		}
		else {
			buffer++;
			entries++;
		}
	} 
	while (contID != 0 && entries < maxentries);
	return entries;
}

void count_memory(void)
{
	register uint64_t *mem;
	uint64_t mem_count, a;
	uint16_t memkb;
	uint8_t irq1, irq2;
	uint64_t cr0;
 
	/* save IRQ's */
	irq1=inb(0x21);
	irq2=inb(0xA1);
 
	/* kill all irq's */
	outb(0x21, 0xFF);
	outb(0xA1, 0xFF);
 
	mem_count=0;
	memkb=0;
 
	// store a copy of CR0
	__asm__ __volatile("movl %%cr0, %%eax":"=a"(cr0))::"eax");
 
	// invalidate the cache
	// write-back and invalidate the cache
	__asm__ __volatile__ ("wbinvd");
 
	// plug cr0 with just PE/CD/NW
	// cache disable(486+), no-writeback(486+), 32bit mode(386+)
	__asm__ __volatile__("movl %%eax, %%cr0", ::
		"a" (cr0 | 0x00000001 | 0x40000000 | 0x20000000) : "eax");
 
	do {
		memkb++;
		mem_count += 1024*1024;
		mem= (uint64_t*) mem_count;
 
		a= *mem;
		*mem= 0x55AA55AA;
 
          // the empty asm calls tell gcc not to rely on what's in its registers
          // as saved variables (this avoids GCC optimisations)
		asm("":::"memory");
		if (*mem!=0x55AA55AA) mem_count=0;
		else {
			*mem=0xAA55AA55;
			asm("":::"memory");
			if(*mem!=0xAA55AA55)
			mem_count=0;
		}
 
		__asm__("":::"memory");
		*mem=a;
 
	} while (memkb<4096 && mem_count!=0);
 
	__asm__ __volatile__("movl %%eax, %%cr0", :: "a" (cr0) : "eax");
 
	mem_end = memkb<<20;
	mem = (uint64_t*) 0x413;
	bse_end= (*mem & 0xFFFF) <<6;
 
	outb(0x21, irq1);
	outb(0xA1, irq2);
}
 
// in your main routine - memory map is stored in 0000:1000 - 0000:2FFF for example
/*[...] {
	[...]
	SMAP_entry_t* smap = (SMAP_entry_t*) 0x1000;
	const int smap_size = 0x2000;
 
	int entry_count = detectMemory(smap, smap_size / sizeof(SMAP_entry_t));
 
	if (entry_count == -1) {
		// error - halt system and/or show error message
		[...]
	} else {
		// process memory map
		[...]
	}
}*/
