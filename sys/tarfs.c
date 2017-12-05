#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/elf64.h>
#include <sys/file.h>
#include <stdlib.h>
#include <sys/string.h>
#include <sys/tarfs.h>
#include <sys/process.h>
#include <sys/mem.h>
static struct posix_header_ustar *headers[200];
static int fc = 0;
unsigned int getsize(const char *in)
{

        unsigned int size = 0;
        unsigned int j;
        unsigned int count = 1;

        for (j = 11; j > 0; j--, count *= 8)
                size += ((in[j - 1] - '0') * count);

        return size;

}

void read_elf(uint64_t add){
        struct Elf64_Ehdr* elf_hdr = (struct Elf64_Ehdr*)add;
        uint64_t phdr;
        if(elf_hdr->e_phoff > 0){
                phdr = add + (elf_hdr->e_phoff);
                struct Elf64_Phdr* phdr1 = (struct Elf64_Phdr*)phdr;
                if(phdr1 != NULL){
                        kprintf("Whatever\n");
                }
                kprintf("%d\n",(phdr1->p_type));
        }
}
uint64_t get_file_address(char* filename){
        for(int i=0;i<fc;i++){
                struct posix_header_ustar *f = headers[i];
                if(strcmp(filename,f->name) == 0){
                        return (uint64_t)headers[i];//((uint64_t)&_binary_tarfs_start + (sizeof(struct posix_header_ustar)*i));
                }
        }
        return -1;
}
void init_tarfs()
{
        struct posix_header_ustar *header = (struct posix_header_ustar *)&_binary_tarfs_start;
        char* address = &_binary_tarfs_start;
        while(address< &_binary_tarfs_end){
                unsigned int size = getsize(header->size);
                headers[fc++] = header;
                address += ((size / 512) + 1) * 512;

                if (size % 512)
                        address += 512;

                header = (struct posix_header_ustar *)address;
        }
}

struct DIR* opendir_tarfs(char* path)
{
	int f = open_tarfs(path, 0);
	struct DIR *d = (struct DIR*) kmalloc(sizeof(struct DIR*));
	d->fd = f;	
	return d;  
}

int open_tarfs(char* file_path, int flags)
{
        //struct file_t *f;
        struct posix_header_ustar* h = (struct posix_header_ustar*) kmalloc(sizeof(struct posix_header_ustar*));
        char *abs_path = (char*)kmalloc(100);
        *(abs_path+0) = '\0';
        int a=0;
        int flag = 0;
        int i=0;
        while(*(file_path+i+1))
        {
                if(*(file_path+i)=='.' && *(file_path+i+1)=='.')
                {
                        a--;
                        *(abs_path+a)='\0';
                        while(*(abs_path+a)!='/')
                        {
                                *(abs_path+a)='\0';
                                a--;
                        }
                        *(abs_path+a)='\0';
                        i++;
                }
                else
                {
                        *(abs_path+a) = *(file_path+i);
                        a++;
                }
                i++;
        }
        *(abs_path+a) = *(file_path+i);
        *(abs_path+a+1) = '\0';
        //kprintf("ABS: %s :PATH", abs_path);
        if(*file_path!='/')
        {
		file_path = abs_path;
                //file_path = strcat(r->curr_dir, abs_path);
        }
        else
        {
                file_path = abs_path+1;
        }
        for(i=0; i<32 && headers[i]!=NULL; i++)
        {
                //kprintf("%s ",headers[i]->name);
                if(strcmp(headers[i]->name,file_path)==0)
                {
                        //kprintf("MATCHED %s ",headers[i]->name);
                        h = headers[i];
                        flag = i;
                        break;
                }
        }
        if(flag == 0){
            return  -1;
        }
       // f = (file_t*) kmalloc(sizeof(struct file_t));	
	int fdc = r->fd_c + 3;
	r->fd_c++;
	strcpy(&(r->fd[fdc].file_name[0]), h->name);
        //r->fd[fdc].offset = (off_t)headers[i+1];
        r->fd[fdc].flags = flags;
	r->fd[fdc].entry = 0;
    	r->fd[fdc].size = (uint64_t)(octal_to_binary((char*)(h->size)));
    	r->fd[fdc].address = (uint64_t)headers[flag];
	r->fd[fdc].fd = fdc;	
        //kprintf("\nF->OFFSET%d ",f->offset);
        //kprintf("\nF->SIZE%d ",f->size);
        //kprintf("\nFile Address: [%p]\n",f->address);
    return fdc;
}

ssize_t read_tarfs(int fd, char* buf, int count)
{
        if(count==0)
        {
                return 0;
        }
	
	 struct file_t* f = (struct file_t*) &(r->fd[fd]);
        //size_t read;
        struct posix_header_ustar *header;
        header = (struct posix_header_ustar*) f->address;
        char* start_address = (char *) (header+1);
        //kprintf("\nSTART ADDRESS%p", start_address);
        //char* temp = buf;
        int i=0;
        if((f->size)<count)
        {
                count = f->size;
        }
        for(i=0; i<count; i++)
        {
		    buf[i] = *(start_address+i);
	    }
        buf[count] = '\0';
	return count;
}

int readdir_tarfs(int fd, char* buf)
{
        int ret = 0;
        int i=0;
	int count = 0;
	char* dir_name = r->fd[fd].file_name;
        for(i=0; i<32 && headers[i]!=NULL; i++)
        {
                //kprintf("%s ",headers[i]->name);
		int index = starts_with(headers[i]->name,dir_name);
                if(index>0)
                {
			//kprintf("MATCHED %s ",headers[i]->name);
			if(count==0)
			{
				if(r->fd[fd].entry==0)
				{
					r->fd[fd].entry++;
				}
			}
			else if(count==r->fd[fd].entry)
			{
				strcpy(buf,substring(headers[i]->name,index));
				kprintf("\n%sEND", buf);
				r->fd[fd].entry++;
				ret = 1;
				break;
			}
			count++;
		}
        }
	return ret;
}

int closedir_tarfs(DIR *d)
{
	return close_tarfs(d->fd);
}

int close_tarfs(int fp)
{
	struct file_t ft = r->fd[fp];
	ft.file_name[0] = '\0';
	ft.offset = 0;
	ft.size = 0;
	//.address = NULL;
	return ft.fd;  
}

uint64_t octal_to_binary(const char* octal)
{
	int oct=0;
	while(*octal)
	{
		oct = (*octal-'0')+(oct*10);
		octal++;
	}
	int octal_dict[8] = {0,1,10,11,100,101,111};
	uint64_t bin=0;
	int i=1;
	int temp=0;
	while(oct!=0)
	{
		temp = oct%10;
		bin += octal_dict[temp]*i;
		i *= 1000;
		oct /= 10;
	}
	return bin;
}

//checks if string1 starts with string2
int starts_with(char* string1, char* string2)
{
	int count = 0;
	while(*string1==*string2 && *string1!='\0')
	{
		count++;
		string1++;
		string2++;
	}
	return count-1;
}

char* substring(char* string, int index)
{
	int i = 0;
	while(*string)
	{	
		if(i>index)
		{
			break;
		}
		string++;
		i++;
	}
	return string;
}
