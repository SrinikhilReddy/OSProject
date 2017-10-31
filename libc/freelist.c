#include<sys/defs.h>

typedef struct freelist_node {
	size_t size;
	void *memory;
	struct freelist *next;
}freelist_node;

static freelist_node* f = NULL;

void* alloc(size_t size) {


}

void free(void *memory) {


}
