#include <stdio.h>
#include <stdlib.h>
#include <string.h>

<<<<<<< HEAD
int main(int argc, char *argv[], char *envp[]) {
  puts("sbush> ");
  return 0;
=======
void readInput();
void parseInput();
//char *commands[] = { "cd","pipe" }
char *in ; //= malloc(sizeof(char)*1000);
char **args; //=malloc(sizeof(char)*1000*1000);
char *command;

//int main(int argc, char* argv[]) {
int main(int argc, char *argv[], char *envp[]) { 
 	while(1){
		puts("sbush> ");
		in = malloc(sizeof(char)*1000); 	
		readInput();
		if(strcmp(in,"exit") == 0){
			puts("Exiting.....");
			break;	
		}
	}
	free(in);
//	free(args);
//	free(command);
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
	
>>>>>>> b60ddbcf97f2093aa7c4e820a419d6b1655fd6db
}
