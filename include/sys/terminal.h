#ifndef _TERMINAL_H
#define _TERMINAL_H

void write_terminal();
void clrscr();
static int no_lines;
char buf[4096];
int getoffset();
void setoffset(int i);
#endif

