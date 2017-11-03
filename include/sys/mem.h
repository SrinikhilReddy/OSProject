#ifndef _PHY_H
#define _PHY_H
typedef struct freelist{
	uint64_t* address;
	struct freelist* next; 
}freelist;
typedef struct smap_t {
    uint64_t base, length;
    uint32_t type;
}__attribute__((packed)) smap_t;
void mem_map(smap_t* sm, uint64_t physbase, uint64_t physfree);
#endif
