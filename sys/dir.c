#include <sys/kprintf.h>
#include <sys/defs.h>
#include <dirent.h>
#include <sys/file.h>
#include <stdio.h>
#include <string.h>
#include <sys/tarfs.h>
#include <sys/mem.h>
#include <sys/process.h>

/*DIR* opendir(const char *name)
{
	if(!name)
	{
		return NULL;
	}
	struct DIR* d;
	int fd = open(name, O_RDONLY|O_DIRECTORY);
	if(fd<0)
	{
		return NULL;
	}
	d = (DIR*) kmalloc(sizeof(*d));
	d->fd = fd;
	return d;
}*/

/*struct dirent* readdir(void *dirp)
{
	//int count = 0;
	struct dir_stream* ds = dirp;
	struct dirent *dp = NULL;
	dp = (struct dirent*)(ds->buf + ds->offset);
	ds->offset = ds->offset + dp->d_reclen;
	return dp;
}*/

/*int closedir(DIR *dirp)
{


}*/

/*uint64_t sys_open(char* path, int flags)
{
	if(!path)
	{
		return 0;
	}
	int fd_last = get_fd(r);
	if(fd_last < 0)
	{
		return fd_last;
	}
	char* curr_dir;
	curr_dir = r->curr_dir;
	char* abs_path="";
	strcpy(abs_path, curr_dir);
	abs_path = strcat(abs_path, path);
	struct file_t* f = open_tarfs(abs_path, flags);
	r->fd[fd_last] = f;
	return fd_last;
}

int get_fd(struct task_struct* task)
{
	int i=0;
	while(i<100)
	{
		if(!task->fd[i])
		{
			return i;
		}	
	}
	return -1;//no file. Error flag yet to be done
}*/
