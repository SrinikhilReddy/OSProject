#ifndef _STDIO_H
#define _STDIO_H

static const int EOF = -1;

typedef struct file
{
        int fd;
}FILE;  

enum flag
{
        READ=1,
        WRITE=2
};


int putchar(int c);
int puts(const char *s);
int printf(const char *format, ...);

char *gets(char *s);
char fgetchar(FILE *f);
int getchar();

FILE* fopen(char *name, char *mode);
int open(const char *path,int flags);
int dup2(int fd1, int fd2);
#endif
