#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readInput();
void parseInput();
//char *commands[] = { "cd","pipe" }
char *in ; //= malloc(sizeof(char)*1000);
char **args; //=malloc(sizeof(char)*1000*1000);
char *command;

int main(int argc, char *argv[], char *envp[]) { 
 	while(1){
		puts("sbush> ");
		in = malloc(sizeof(char)*1000); 	
		readInput();
		if(strcmp(in,"exit") == 0){
			puts("Exiting.....");
			break;	
		}
		parseInput();
	}
	free(in);
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
	char *temp = strtok(in," ");
	command = temp;
	int i=0;
	args =  malloc(sizeof(char)*1000*1000);

	temp = strtok(NULL," ");
	while(temp!= NULL){
		args[i] = temp;
		temp = strtok(NULL," ");
	}
}
