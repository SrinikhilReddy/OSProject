//#include <sys/kprintf.h>
#include <stdarg.h>
va_list list;
static char *p_reg = (char*)0xb8000;
static int h_offset=0,v_offset=0;

void parse_str(const char* fmt);
void kprintf(const char* fm, ...);
void print_int(int i);
void put_to_screen(char a);
void scroll();

void scroll(){
	char* p  = (char*)0xb8000;
	for(int i=0;i<24;i++){
		for(int j=0;j<160;j=j+2){
			int prev = (i*160)+j;
			int next = (i+1)*160 + j;
			*(p+prev) = *(p+next);
		}
	}
	p_reg = (char*)(0xb8000);
	p_reg = p_reg+(160*24);
	for(int i=0;i<160;i=i+2){
		*(p_reg+i) = '\0';
	}
	p_reg = (char*)(0xb8000)+(160*24);
}
void put_to_screen(char a){
	if(a == '\n'){
		p_reg =(char*)(0xb8000);
		int k = 80*(v_offset+1);
		for(int i=0;i<k;i++){
			p_reg+=2;
		}
		if(v_offset == 24){
			scroll();	
		}
		else{
			v_offset++;
		}
		h_offset = 0;
		return;
	}
	else if(a == '\r'){
		p_reg = (char*)(0xb8000);
		int k = 80*(v_offset);
		for(int i=0;i<k;i++){
			p_reg+=2;
		}
		h_offset=0;
	}
	else{
		*p_reg = (char)a;

		p_reg+=2;
	}
	if(h_offset == 79){
		if(v_offset == 24){
			//scroll
			scroll();
			p_reg = (char*)0xb8000;
			p_reg = p_reg + 160*23;
			h_offset = 0;
			return;
		}
		else{
			v_offset++;
			h_offset = 0;
			return;
		}
	}
	h_offset++;

}
void print_int(int i){
	int j = 1000000000;
	int lead_zero = 1;
	if(i<0){
		put_to_screen('-');
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
		put_to_screen('0'+(i/j));
		i = i % j;
		j = j/10;
	}
}
void print_hex(int i){
	char a[11];
	int l=0;
	while(i!=0){
		int k = i%16;
		if(k<9){
			a[l++] = '0'+k;
		}
		else{
			a[l++] = 'A'+(k-10);
		}
		i = i/16;
	}
	for(int k=l-1;k>=0;k--){
		put_to_screen(a[k]);	
	}

}
void kprintf(const char *fmt, ...)
{
	va_start(list,fmt);
	parse_str(fmt);
}

void parse_str(const char* s){
	while(*s!='\0'){
		if(*s == '%'){
			switch(*(s+1)){
				case 'd':
					print_int(va_arg(list,int));
					s = s+2;
					break;
				case 'c':
					put_to_screen(va_arg(list,int));
					s = s+2;
					break;
				case 's':
					;
					char* a;
					a = va_arg(list,char *);
					while(*a!='\0'){
						put_to_screen(*a);
						a++;
					}
					s+=2;
					break;
				case 'x':
					print_hex(va_arg(list,int));
					s = s+2;
					break;
				default:
					break;	
			}
		}
		else if(*s == '\\'){
			switch(*(s+1)){
				case 'n':
					put_to_screen('\n');
					s+=2;
					break;
				case 'r':
					put_to_screen('\r');
					s+=2;
					break;
			}
		}
		else{
			put_to_screen(*s);
			s++;
		}
	}
}
