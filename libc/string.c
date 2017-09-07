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

void strcpy(char *string2, char *string1){

	while(*string1)
	{
		*string2=*string1;
		string1++;
		string2++;
	}
	*string2='\0';
}

int strlen(const char *string)
{
	int length=0;
	while(*string)
	{
		length++;
		string++;
	}
	return length;
}

char* strcat(char *string1, char *string2)
{
	while(*string1!='\0')
	{
		string1++;
	}
	while(*string2!='\0')
	{
		*string1=*string2;
		string1++;
		string2++;
	}
	*string1='\0';
	return string1;
}
