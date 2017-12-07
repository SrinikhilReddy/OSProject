#include "../include/syscall.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

va_list list;
void print_pointer(unsigned long i);
void print_hex(int i);
void print_int(int num);
void string_split(const char* str, char s[100][100]);

int printf(const char* str,...)
{
	va_start(list,str);
	char s[100][100];
	string_split(str, s);
	for(int i=0; i<100&&strcmp(s[i],"\0")!=0; i++)
	{
		if(strcmp(s[i],"\n")==0)
		{
			puts(s[i]);
		}
		else if(strcmp(s[i],"%d")==0)
		{ 
			print_int(va_arg(list,int));
		}
		else if(strcmp(s[i],"%s")==0)
		{
			char* a = (char*) va_arg(list,char *);
			puts(a);
		}
		/*else if(strcmp(s[i],"%c")==0)
		{
			char* a = (char*) va_arg(list,int);
			puts(a);
		}*/
		else if(strcmp(s[i],"%x")==0)
		{
			print_hex(va_arg(list,int));
		}
		else if(strcmp(s[i],"%p")==0)
		{
			puts("0");
			puts("x");
			unsigned long p  = va_arg(list,unsigned long);	
			print_pointer(p);
		}
		else
		{
			puts(s[i]);
		}
		
	}
	return 0;
}

void print_int(int num)
{
	char nums[10] = "0123456789";
	char buf[100];
	char* p = buf;
	int temp = num;
	do
	{	
		++p;
		temp = temp/10;
	}while(temp);
	*p = '\0';
	do
	{
		*--p =  nums[num%10];
		num = num/10;
	}while(num);
	puts(buf);
}

void print_pointer(unsigned long i){
	    char a[100];
        int l=0;
        while(i!=0){
                int k = i%16;
                if(k<10){
                        a[l++] = '0'+k;
                }   
                else{
                        a[l++] = 'A'+(k-10);
                }   
                i = i/16;
        }
        a[l]='\0';
     /*   for(int k=l-1;k>=0;k--){
		        char* t = &a[k];
                puts(t);    
        }*/
        puts(a);
}
void print_hex(int i){
	char a[100];
	int l=0;
	while(i!=0){
		int k = i%16;
		if(k<=9){
			a[l++] = '0'+k;
		}
		else{
			a[l++] = 'A'+(k-10);
		}
		i = i/16;
	}
	for(int k=l-1;k>=0;k--){
		char* t = &a[k];
		puts(t);	
	}

}

void string_split(const char* str, char s[100][100])
{
    char temp[50];
    int i=0,j=0;
    while(*str)
    {
        if(*str=='%')
        {
            str++;
	    if(strcmp(temp,"\0")!=0)
	    {
		strcpy(s[i],temp);
            	i++;
            	for(int k=0; k<=j; k++)
            	{
                	temp[k]='\0';
            	}
	    }
            temp[0] = '%';
            temp[1] = *str;
            strcpy(s[i],temp);
            i++;
            temp[0] = '\0';
            temp[1] = '\0';
            j=0;        }
        else if(*str=='\n' || *str=='\t')
        {
	    if(strcmp(temp,"\0")!=0)
	    {
            	strcpy(s[i],temp);
	    }
            i++;
            for(int k=0; k<=j; k++)
            {
                temp[k]='\0';
            }
            temp[0] = *str;
            strcpy(s[i],temp);
            i++;
            temp[0] = '\0';
            j=0;
        }
        else
        {
            temp[j] = *str;
            j++;
        }
        str++;
    }
    strcpy(s[i],temp);
}
