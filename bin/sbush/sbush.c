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
	free(in);
	free(args);
	free(command); 	
	return 0;
}

void execCommand(){
//	puts(command);
//	for(int i=0;i<(sizeof(commands)/sizeof(char));i++){	
//Need better if checks
		if( /*(strcmp(command,commands[i]) == 0) &&*/ (strcmp(command,"cd") == 0)){
			/*	if(args==NULL){	
					puts("NULL");
					args[0]="";
				}	*/
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
//		puts(args[i]);
		}
}

void chdir_1(char **args){
	//	puts(args[0]);
		// Input validation for args need to be done
		// Error cases to be handled	
	if(args[0] == NULL){
		puts("No argument passed");
		return;
	}
//	int i = chdir(args[0]);
	if(chdir(args[0])!=0){
		puts("Invalid CD usage. Check path and argument passed");
	}
	else{
	puts("success");
	}
}
