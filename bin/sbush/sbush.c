#include <stdio.h>
#include <stdlib.h>


void readInput();
char *in ; //= malloc(sizeof(char)*1000);

int main(int argc, char* argv[]) {
  	puts("sbush> ");
	in = malloc(sizeof(char)*1000); 	
	readInput();
	
	//	puts(in);
 	 return 0;
}

void readInput(){
	char c;
	int i = 0;
	while( (c=getchar())!= '\n'){
		in[i] = c;
		i++;
	}
}

void parseInput(){
	
}
