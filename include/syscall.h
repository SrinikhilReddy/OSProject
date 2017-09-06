#ifndef _SYSCALL_H
#define _SYSCALL_H
#include "syscall_const.h"

#define _sys_return(type,res) \
do{ 				\
	return (type)(res); \
}while(0) 

#define _syscall(type,name) \
do{ \
long __res; \
__asm__ volatile (  "movq %1, %%rax; syscall; movq %%rax, %0;" \
                  : "=m" (__res) \
                  : "g" (__NR_##name) \
		  : "rax"); \
 return (type) (__res); \
}while(0)

#define _syscall1(type,name,type1,arg1) \
do{ \
long __res; \
__asm__ volatile (  "movq %1, %%rax ; movq %2, %%rdi; syscall; movq %%rax, %0;" \
                  : "=m" (__res) \
                  : "g" (__NR_##name),"g" ((long)(arg1)) \
		  : "rax","rdi" );\
 return (type) (__res); \
}while(0)

#define _syscall2(type,name,type1,arg1,type2,arg2) \
do{\
long __res; \
__asm__ volatile (  "movq %1, %%rax ; movq %2, %%rdi; movq %3, %%rsi; syscall; movq %%rax, %0;" \
                  : "=m" (__res) \
                  : "g" (__NR_##name),"g" ((long)(arg1)),"g" ((long)(arg2)) \
		  : "rax","rdi","rsi"); \
 _sys_return(type,__res); \
}while(0)

#define _syscall3(type,name,type1,arg1,type2,arg2,type3,arg3) \
do{ \
long __res; \
__asm__ volatile (  "movq %1, %%rax ; movq %2, %%rdi; movq %3, %%rsi; movq %4, %%rdx; syscall; movq %%rax, %0;" \
                  : "=m" (__res) \
                  : "g" (__NR_##name),"g" ((long)(arg1)),"g" ((long)(arg2)), "g" ((long)(arg3)) \
		  : "rax","rdi","rsi","rdx"); \
 return (type) (__res); \
}while(0)

#define _syscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4)\
do{ \
long __res; \
__asm__ volatile (  "movq %1, %%rax ; movq %2, %%rdi; movq %3, %%rsi; movq %4, %%rdx; movq %5, %%r10; syscall; movq %%rax,%0" \
                  : "=m" (__res) \
                  : "g" (__NR_##name),"g" ((long)(arg1)),"g" ((long)(arg2)), "g" ((long)(arg3)), "g" ((long)(arg4)) \
                  : "rax","rdi","rsi","rdx","r10"); \
 return (type) (__res); \
}while(0)
#endif
