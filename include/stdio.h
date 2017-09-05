#ifndef _STDIO_H
#define _STDIO_H

static const int EOF = -1;

typedef struct file
{
        int char_count;
        int access_mode;
        int fdesc;
        char *base;
        char *next;
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

#endif
