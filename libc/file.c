#include "../include/syscall.h"
#include "../include/stdlib.h"
#include "../include/sys/defs.h"
#include "../include/sys/file.h"

file* open(char* file_name) {
	_syscall1(file*, open, char*, file_name);
	return 0;
	//return (file*) addr;		
}

uint64_t open_dir(char* name) {
	_syscall1(uint64_t, open_dir, char*, name);
	return 0;
	//return addr;
}

uint64_t read_dir(DIR* d) {
	_syscall1(uint64_t, read_dir, DIR*, d);
	return 0;
	//return addr; 
} 

