#include<stdio.h>


//lexicographic comparison
int strcmp(char *s,char *t){
	while(*s==*t)
	{
		if(*s=='\0')
			return 0;
		s++;
		t++;
	}
	return *s-*t;
}

char* strtok(char *string, const char *d){
	
	static char *s;
	if(string!=NULL)
	{
		s=string;
	}
	static int index_s;
	int index_d=0;
	while(s[index]!='\0')
	{	index_d=0;
		while(d[j]!='\0')
		{
			if(s[index_s]==d[index_d])
			{
				
			}
			index_s++;	
		}
		
	}
	
	
}
