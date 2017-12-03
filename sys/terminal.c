#include <sys/kprintf.h>
#include <sys/defs.h>
#include <syscall.h>
#include <sys/idt.h>
#include <sys/terminal.h>

//Reference://scancode array:http://www.cs.umd.edu/~hollings/cs412/s98/project/proj1/scancode
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
       { '^','M' } ,
       { ' ','^' } ,
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

char buf[4096];
static int caps=0;
static int ctrl=0;
static int i=0;
static uint8_t inb(uint64_t port);
void write_terminal()
{
	char key_pressed;
	int c=0;
	//while(1)
	//{
		if(inb(0x60)!=0){
                	c=inb(0x60);
		}
		if(c>0)
		{
			if(c>58)
			{
				outportb(0x20, 0x20);
				return;
			}
			if(c==14)//Backspace
			{
				*(buf+i-1) = '\0';
				i--;
			}
			else if(c==28)
			{
				kprintf("\n");
				//_syscall3(int,write,int,stdout,int*,buf,int,i+1);
				//syscall for write
				for(int j=0; j<=i; j++)
				{
					*(buf+j)='\0';
				}
				i = 0;
				clrscr();
				outportb(0x20, 0x20);
				return;
			}
			else if(c==29)
			{
				ctrl=1;
				kprintf("%c",code_map[c][ctrl]);
				*(buf+i) = code_map[c][ctrl];
				i++;
			}
			else if(c==42||c==54)
			//else if(c==32)
			{
				caps=1;
			}
			else
			{
				/*key_pressed=code_map[c][caps];
				caps=0;
				*reg=key_pressed;*/
				if(ctrl==1)
				{	
					ctrl=0;
					caps=1;	
				}	
				else
				{
					/*kprintf("%c",code_map[29][ctrl]);
					*buf = code_map[29][ctrl];
					buf++;*/
				}
				key_pressed=code_map[c][caps];
				caps=0;
				kprintf("%c",key_pressed);
				*(buf+i) = key_pressed;
				i++;			
			}	
		}		
	//}
	outportb(0x20,0x20);	
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

void clrscr()
{
	char *p_reg = (char*) 0xffffffff800b8000;
        for(int i=0;i<25;i++){
		for(int j=0; j<80; j++){
			*p_reg = '\0';
        		p_reg+=2;
		}
        }
}
