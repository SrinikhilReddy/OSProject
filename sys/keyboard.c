#include<../sys/idt.h>
#include<sys/kprintf.h>
#include<../sys/idt.h>

static char code_map[58][2] =
     {
       {   0,0   } ,
       {   0,0   } ,//ESC
       { '1','!' } ,
       { '2','@' } ,
       { '3','#' } ,
       { '4','$' } ,
       { '5','%' } ,
       { '6','^' } ,
       { '7','&' } ,
       { '8','*' } ,
       { '9','(' } ,
       { '0',')' } ,
       { '-','_' } ,
       { '=','+' } ,
       {   8,8   } ,
       {   9,9   } ,
       { 'q','Q' } ,
       { 'w','W' } ,
       { 'e','E' } ,
       { 'r','R' } ,
       { 't','T' } ,
       { 'y','Y' } ,
       { 'u','U' } ,
       { 'i','I' } ,
       { 'o','O' } ,
       { 'p','P' } ,
       { '[','{' } ,
       { ']','}' } ,
       {  13,13  } ,
       {   0,0   } ,
       { 'a','A' } ,
       { 's','S' } ,
       { 'd','D' } ,
       { 'f','F' } ,
       { 'g','G' } ,
       { 'h','H' } ,
       { 'j','J' } ,
       { 'k','K' } ,
       { 'l','L' } ,
       { ';',':' } ,
       {  39,34  } ,
       { '`','~' } ,
       {   0,0   } ,
       { '\\','|'} ,
       { 'z','Z' } ,
       { 'x','X' } ,
       { 'c','C' } ,
       { 'v','V' } ,
       { 'b','B' } ,
       { 'n','N' } ,
       { 'm','M' } ,
       { ',','<' } ,
       { '.','>' } ,
       { '/','?' } ,
       {   0,0   } ,
       {   0,0   } ,
       {   0,0   } ,
       { ' ',' ' } ,
   };


static char *reg = (char*)0xB8F9E;

static uint8_t inb(uint64_t port);

void kb()
{
	char key_pressed;
	int c=0;
	int caps=0;
	//while(1)
	{
		if(inb(0x60)!=0)
			c=inb(0x60);
		if(c>0)
		{
			if(c==42||c==54)
			{
				caps=1;
				continue;
			}
			else
			{
				key_pressed=code_map[c][caps];
				caps=0;
				*reg=key_pressed;						
			}	
		}		
	}	
}

static inline uint8_t inb(uint64_t port)
{
	uint8_t r;
	__asm__ volatile( 
			"inb %1, %0"
			: "=a"(r)
			: "Nd"(port)
       		        );
	return r;
}
