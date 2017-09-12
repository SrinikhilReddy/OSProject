//#include <sys/kprintf.h>
#include <stdarg.h>
va_list list;
void parse_str(const char* fmt);
void kprintf(const char* fm, ...);
char* print_int(register char* no,int i);

char* print_int(register char* no,int i){
	int j = 1000000000;
	int lead_zero = 1;
	if(i<0){
		*no = '-';
		no+=2;
		i = i* -1;
	}
	while(j!=0){
		if(lead_zero == 1 && (i/j == 0)){
			j = j/10;
			continue;
		}
		if(lead_zero == 1 && (i/j != 0)){
			lead_zero = 0;	
		}
		*no = (char)('0'+(i/j));
		i = i % j;
		j = j/10;
		no +=2;
	}
	return no;
}
void kprintf(const char *fmt, ...)
{
	va_start(list,fmt);
	//print_val = (char*)0xb8000;
	parse_str(fmt);
}

void parse_str(const char* s){
	int i;
	register char *print_val;
	print_val = (char*)0xb8000;
	while(*s!='\0'){
	if(*s == '%'){
		switch(*(s+1)){
			case 'd':
				i = va_arg(list,int);
				print_val = (char*)print_int(print_val,va_arg(list,int));
				s = s+2;
				break;	
		}
	}
	else{
	   *print_val = *s;
	   s++;
	   print_val+=2;
	}
	}
}

