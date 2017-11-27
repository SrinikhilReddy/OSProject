#ifndef _DEFS_H
#define _DEFS_H

#define NULL ((void*)0)

#define stdin 0
#define stdout 1
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#define O_RDONLY 00
#define O_DIRECTORY 00200000
typedef unsigned long  uint64_t;
typedef          long   int64_t;
typedef unsigned int   uint32_t;
typedef          int    int32_t;
typedef unsigned short uint16_t;
typedef          short  int16_t;
typedef unsigned char   uint8_t;
typedef          char    int8_t;

typedef uint64_t size_t;
typedef int64_t ssize_t;

typedef uint64_t off_t;

typedef uint32_t pid_t;

typedef uint16_t mode_t;

void *memset(void *s, int c, size_t n); 
#endif
