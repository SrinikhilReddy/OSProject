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
	int i=index_s;
	while(s[index_s]!='\0')
	{	index_d=0;
		while(d[index_d]!='\0')
		{
			if(s[index_s]==d[index_d])
			{
				s[index_s]='\0';
			}
			index_s++;	
			if(s[i]!='\0')
			{
				return &s[i];
			}
			else
			{
				i=index_s;
				index_s--;
				break;
			}	
			index_d++;
		}
		index_s++;
	}
	s[index_s]='\0';
	if(s[i]=='\0')
		return NULL;
	else
		return &s[i];
	
}

void strcpy(char *string2, char *string1){

	while(*string1)
	{
		*string2=*string1;
		*string1++;
		*string2++;
	}
	*string2='\0';
}
