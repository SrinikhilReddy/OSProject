#include <stdlib.h>
int *adr;
char **adr2; 
char* argv[20],*env[100];
void _start(void) {
  __asm__ __volatile__ ("movq %%rsp,%0; movq %%rsp,%1"
	 :"=m" (adr),"=m" (adr2)
	 :
	 :"rsp"	
	);
  int k=0;
  adr2 = adr2 + 2;
  //adr2 = adr+3;
  for( k=0;k<*(adr+2);k++)
  {
	argv[k] = (*(adr2++));	
  }
  k=0;
  while((*adr2)!=NULL){
	env[k++] = (*(adr2++));
  }
  //while(env[k-1]!=NULL);
 // pushenvs(env);
  int i = main(0,NULL,NULL);
  // call main() and exit() here
  exit(i);
}
