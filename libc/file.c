#define<syscall.h>
#define<stdlib.h>
#define<sys/defs.h>
#define<file.h>

file* open(char* file_name) {
	uint64_t addr = syscall1(file*, open, char*, file_name);
	return (file*) addr;		
}

uint64_t open_dir(char* name) {
	uint64_t addr = syscall1(uint64_t, open_dir, char*, name);
	return addr;
}

uint64_t read_dir(DIR* d) {
	uint64_t addr = syscall1(uint64_t, read_dir, DIR*, d);
	return addr; 
} 

