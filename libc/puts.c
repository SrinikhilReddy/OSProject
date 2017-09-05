#include <syscall.h>
#include <stdio.h>
#include <stdlib.h>
int puts(const char *s)
{
  for( ; *s; ++s) if (putchar(*s) != *s) return EOF;
  return (putchar('\n') == '\n') ? 0 : EOF;
}
