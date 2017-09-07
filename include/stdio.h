#ifndef _STDIO_H
#define _STDIO_H
#include <sys/defs.h>

static const int EOF = -1;
struct dirent_n {
           long           d_ino;
           off_t          d_off;
           unsigned short d_reclen;
           char           d_name[];
};  
typedef struct file
{
        int fd;
}FILE;  

enum flag
{
        READ=1,
        WRITE=2
};


int fputchar(int c,FILE* fp);
int putchar(int c);
int puts(const char *s);
int printf(const char *format, ...);
off_t lseek(int fd, off_t offset, int p);
char *gets(char *s);
char fgetchar(int fd);
int getchar();
char* fgets(char* s, int n,FILE *f);
FILE* fopen(char *name, char *mode);
int open(const char *path,int flags);
int close(int fd);
int dup2(int fd1, int fd2);
#endif
