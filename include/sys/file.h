#ifndef _FILE_H
#define _FILE_H
#include<sys/defs.h>

/*typedef struct {


} file_system;*/

typedef struct {
	char file_name[100];
	uint64_t inode_num;
	uint64_t size;
	uint64_t address;
} file;

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

file* open(char* file_name);

#endif
