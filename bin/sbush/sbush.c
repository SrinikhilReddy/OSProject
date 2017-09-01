#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void readInput();
void parseInput();
void execCommand();
void chdir_1(char **args);
char *commands[] = { "cd" };
char *in ; //= malloc(sizeof(char)*1000);
char **args; //=malloc(sizeof(char)*1000*1000);
char *command;

int main(int argc, char *argv[], char *envp[]) { 
//Have to modify to implement script execution, when file name is passed as an argument
 	while(1){
		puts("sbush> ");
		in = malloc(sizeof(char)*1000); 	
		readInput();
		if(strcmp(in,"exit") == 0){
			puts("Exiting.....");
			break;	
		}
		parseInput();
		execCommand();
	}
	free(in);
 	return 0;
}

void execCommand(){
	puts(command);
//	for(int i=0;i<(sizeof(commands)/sizeof(char));i++){	
//Need better if checks
		if( /*(strcmp(command,commands[i]) == 0) &&*/ (strcmp(command,"cd") == 0)){
			//Need to check if this is really required 
			chdir_1(args);	
		}
//	}
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

void chdir_1(char **args){
	puts("S");
		// Error cases to be handled	
	chdir(args[0]);
}
