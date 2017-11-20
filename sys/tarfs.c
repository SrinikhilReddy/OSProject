#include <sys/tarfs.h>
#include <sys/kprintf.h>

struct posix_header_ustar *headers[32];

unsigned int getsize(const char *in)
{

	unsigned int size = 0;
	unsigned int j;
	unsigned int count = 1;

	for (j = 11; j > 0; j--, count *= 8)
		size += ((in[j - 1] - '0') * count);

	return size;

}

void init_tarfs()
{
	//while(1){ 
	//char address = _binary_tarfs_start;
//	char end_a = _binary_tarfs_end;
	//kprintf("%p , %p\n", &_binary_tarfs_start, &_binary_tarfs_end);
//	address += 512;
	struct posix_header_ustar *header = (struct posix_header_ustar *)&_binary_tarfs_start;
	int i = 0;
//	while(&address < &_binary_tarfs_end){
	char* address = &_binary_tarfs_start;
	while(address< &_binary_tarfs_end){
	
		kprintf("%s ",header->name);
		unsigned int size = getsize(header->size);
		kprintf("%d, %d \n",i, size);
		headers[i++] = header;

		address += ((size / 512) + 1) * 512;

		if (size % 512)
			address += 512;

		header = (struct posix_header_ustar *)address;
	}


}
