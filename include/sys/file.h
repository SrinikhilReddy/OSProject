#ifndef _FILE_H
#define _FILE_H
#include<sys/defs.h>

/*typedef struct {


} file_system;*/

typedef struct file_t {
	char file_name[100];
	off_t offset;
	uint64_t flags; 
	uint64_t inode_num;
	uint64_t size;
	uint64_t address;
} file_t;

typedef struct {
	uint64_t size;
	uint64_t magic_no;
	uint64_t inode_start;
	uint64_t block_start;
	uint64_t inode_max;
	uint64_t block_max;
	uint64_t inode_free;
	uint64_t block_free;
} superblock;

typedef struct {
	char file_name[100];
	uint64_t file_size;
	uint64_t block_count;
} inode;

typedef struct {
	struct inode *inode;
	char file_name[100];
} dentry;

//extern int register_filesystem(struct file_system *);
//extern int unregister_filesystem(struct file_system *);

struct file_t* open_tarfs(char* file_path, int flags);
ssize_t read_tarfs(struct file_t* f, char* buf, int count);
uint64_t octal_to_binary(const char* octal);

#endif
