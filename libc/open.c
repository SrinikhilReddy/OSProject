#include <syscall.h>
#include <sys/defs.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

static DIR dir[100];
static struct dirent d[100];
int open(const char *path,int flags);
//FILE filepointer;
int fopen(char *name, char *mode)
{

    int flag;
    if(*mode == 'r'){
        flag = O_RDONLY;
    }   
    int fd = open(name,flag);
    return fd;   
}
int open(const char *path,int flags)
{
	_syscall2(int, open, char*, path, int, flags);
	return 0;
}
int close(int fd){
    _syscall1(int,close,int,fd);
}
int open_dir(const char *path)
{
    _syscall1(int, opendir, char*, path);
}

DIR *opendir(const char *name){
    int l = open_dir(name);
    if(l == -1){
        dir[0].fd = l;
        return &dir[0];
    }
    dir[l].fd = l;
    strcpy(dir[l].d_ent.d_name,(char *)name);
    return &dir[l];
}

int direccall(int fd,char* buff,int size){
    _syscall3(int,getdents,int, fd,char*, buff,int,size);
}
struct dirent *readdir(DIR *dirp){
    int i = direccall(dirp->fd,d[dirp->fd].d_name,4096);
    if(i == 0){
        return NULL;
    }
    return &d[dirp->fd];
}
int closedir(DIR *dirp){
    return close(dirp->fd);
}