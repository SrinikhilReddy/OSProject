
int strcmp_(char *s,char *t){
	while(*s==*t)
	{
		if(*s=='\0')
			return 0;
		s++;
		t++;
	}
	return *s-*t;
}

void strcpy_(char *string2, char *string1){

	while(*string1)
	{
		*string2=*string1;
		string1++;
		string2++;
	}
	*string2='\0';
}

int strlen_(char *string)
{
	int length=0;
	while(*string)
	{
		length++;
		string++;
	}
	return length;
}


