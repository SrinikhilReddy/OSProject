#include<syscall.h>
#include<sys/defs.h>
off_t lseek(int fd, off_t offset, int p)
{
	_syscall3(off_t,lseek,int,fd,off_t,offset,int,p);
	return 0;
}
