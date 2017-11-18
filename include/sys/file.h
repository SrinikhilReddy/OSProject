#ifndef _FILE_H
#define _FILE_H
#define<sys/defs.h>

typedef struct {
	uint64_t size;
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

#endif
