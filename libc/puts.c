#include "../include/syscall.h"

int puts(const char *s)
{
  for( ; *s; ++s) if (putchar(*s) != *s) return EOF;
  return (putchar('\n') == '\n') ? 0 : EOF;
}

int main()
{
	char *s="abcdef";
	puts(s);
}
