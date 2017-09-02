#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


void readInput();
void parseInput();
void execCommand();
void chdir_1(char **args);
void clearInput();
void clearCommand();
void clearArguments();

char input[1025];
char com[1025];
char *arguments[1000];
char *in = &input[0] ; //= malloc(sizeof(char)*1000);
char **args = &arguments[0]; //=malloc(sizeof(char)*1000*1000);
char *command = &com[0];

int main(int argc, char *argv[], char *envp[]) { 
	if(argv[1]== NULL){ 	
		while(1){
			puts("sbush> ");
			readInput();
			if(input[0] == '\0'){
				continue;
			}
			if(strcmp(in,"exit") == 0){
				return 0;	
			}
			parseInput();
			execCommand();

			clearInput();
			clearCommand();
			clearArguments();
		}
	}else{
		FILE *fp;
		fp = fopen(argv[1],"r");
		if(fp == NULL){
			puts("File not found, Exiting!");
			return 0;
		}
		int c,i=0;
		while(c!=EOF){
			c = fgetc(fp);	
			while(c!='\n' && c!=EOF){
				in[i++]=(char) c;
				c = fgetc(fp);
			}	
			parseInput();
			execCommand();			
			i=0;

			clearInput();
			clearCommand();
			clearArguments();
		}
	}
	return 0;
}

void execCommand(){
	int pid;
	if(strcmp(command,"cd") == 0){
		chdir_1(args);	
	}
	else{
		if ((pid = fork ()) == 0) {
			if(execvp(command, args) == -1){
				puts("error");
			}
		}
		else if (pid > 0) {		
			wait (0);
		}
		else {

			perror ("Failed to fork\n");
		}
	}
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
	int i=1;
	args[0] = temp;
	temp = strtok(NULL," ");
	while(temp!= NULL){		
		args[i++] = temp;
		temp = strtok(NULL," ");
	}
}

void chdir_1(char **args){
	if(args[1] == NULL){
		puts("No argument passed");
		return;
	}
	if(chdir(args[1])!=0){
		puts("Invalid CD usage. Check path and argument passed");
	}
}

void clearInput(){
	for(int i=0;i<sizeof(input)/sizeof(char);i++){
		input[i] = '\0';
	}
}

void clearCommand(){
	for(int i=0;i<sizeof(com)/sizeof(char);i++){
		com[i] = '\0';
	}
}

void clearArguments(){
	for(int i=0;i<1000;i++){
		arguments[i] = NULL;		
	}
}
