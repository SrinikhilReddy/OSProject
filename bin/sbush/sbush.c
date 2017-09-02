#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
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
	if(argv[1]== NULL){ 	
		while(1){
			puts("sbush> ");
			in = malloc(sizeof(char)*1000); 	
			readInput();
			if(strcmp(in,"exit") == 0){
				return 0;	
			}
			parseInput();
			execCommand();
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
			in = malloc(1000*sizeof(char));
			c = fgetc(fp);	
			while(c!='\n' && c!=EOF){
				in[i++]=(char) c;
				c = fgetc(fp);
			}	
			puts(in);
			parseInput();
			execCommand();			
			i=0;
		}
	}
//	free(in);
//	free(args);
//	free(command); 	
	return 0;
}

void execCommand(){
	int pid;
	if(strcmp(command,"cd") == 0){
			chdir_1(args);	
	}
	else{
		if ((pid = fork ()) == 0) {
//			puts(";;;;");
	//		puts(command);
	//		puts(args[0]);
	//		args[1] = '\0';
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
//	arg[0] = temp;
	int i=1;
	args =  malloc(sizeof(char)*1000*1000);
	args[0] = temp;
	temp = strtok(NULL," ");
	while(temp!= NULL){		
		args[i] = temp;
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
