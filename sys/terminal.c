#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/process.h>
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
static int offset = 0;
static int caps=0;
static int ctrl=0;
static int i=0;
static uint8_t inb(uint64_t port);
int no_lines;
char buf[4096];

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
                if(offset != i) {
                    *(buf + i - 1) = '\0';
                    i--;
                    backspace();
                }
            }
			else if(c==28)
			{
				kprintf("\n");
                buf[i] = '\n';
                no_lines++;
                i++;
                wake_process();
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
void wake_process(){
    for(int i=0;i<MAX;++i){
        if(q[i].state == SLEEPING){
            q[i].state = RUNNING;
            //      yield();
            return;
        }
    }
}
void read_input(char* b){
    while(1){
        if(no_lines>0){
            int j=0;
            for(int i = getoffset();i<4096;i++,j++){
                if( buf[i] == '\n'){
                    setoffset(i+1);
                    no_lines--;
                    r->state = RUNNING;
                    return;
                }
                *(b+j) = buf[i];
                buf[i] = '\0';
            }
        } else{
            r->state = SLEEPING;
        }
        yield();
    }
}
int getoffset(){
    return offset;
}
void setoffset(int i){
    offset = i;
}

