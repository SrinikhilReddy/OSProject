#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
void readInput();
void parseInput();
void execCommand();
void chdir_1(char **args);
void clearInput();
void clearCommand();
void clearArguments();
void setStringTokens(char* string, char delimiter, char* strs[]);
void initargs();
void setvar(char *args[]);
void forkandExec(char* cmd,char* ag[]);
int getInputArgCounts();

int isBackground;
int argsNo;
char input[1025];
char com[1025];
char arg[1000][1000];
char prompt[1000];
char *in = &input[0] ;
char *args[1000] ; 
char path[50];
char *command = &com[0];
char env_var[1000][1000];
int env_length=0;

int main(int argc, char *argv[], char *envp[]) { 
	initargs();
	strcpy(prompt,"sbush");
	strcpy(path,".");
	int i=0;
	while(envp[i]!=NULL)
	{
		strcpy(&env_var[i][0],envp[i]);
		i++;
	}
	env_length=i;
	if(argc==1){ 	
		while(1){
			puts(prompt);
			puts(">");
			readInput();
			if(input[0] == '\0'){
				continue;
			}
			if((strcmp(in,"exit") == 0) || (strcmp(in,"quit") == 0)){
				return 0;	
			}
			
			parseInput();
			//execCommand();

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
		int c,i=0,fd=fp->fd;
		while(c!=EOF){
			c = fgetchar(fd);	
			while(c!='\n' && c!=EOF){
				in[i++]=(char) c;	
				//lseek(fd,1,SEEK_CUR);		
				c = fgetchar(fd);
			}	
			parseInput();
			//execCommand();			
			i=0;

			clearInput();
			clearCommand();
			clearArguments();
		}
	}
	return 0;
}

void execCommand(){
	if(strcmp(command,"cd") == 0){
		chdir_1(args);	
	}
	else if(strcmp(command,"export") == 0){
		setvar(args);
	}
	else{
		forkandExec(command,args);
	}
}
void forkandExec(char* cmd,char* ag[]){
	pid_t pid;
	char cmd_w_path[50];
	strcpy(&cmd_w_path[0],path);
        if(path[(strlen(path))-1] != '/') {
		strcat(&cmd_w_path[0],"/");
	}
	strcat(&cmd_w_path[0],cmd);
	strcpy(ag[0],cmd);
	if ( (pid = fork()) == 0) {
		if(execvp(cmd_w_path, ag) == -1){
			puts("error");
		}
	}
	else if (pid < 0) {
		puts("Failed to fork\n");
	}
	else {
		if(isBackground == 1){
			return;
		}
		wait(0);
		//puts("Failed to fork\n");
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
	setStringTokens(in,'|',args);
	if(args[1] == NULL){ //Not a pipe command
		clearArguments();
		setStringTokens(in,' ',args);
		command = args[0];
		execCommand();
		return;
	}
	else{   //Pipe Command
		//int argCount = getInputArgCounts();
		char parg1[10][1000],parg2[10][1000];
		char *pargs1[10],*pargs2[10];	
		for(int i=0;i<10;i++){
			pargs1[i] = &parg1[i][0];
			pargs2[i] = &parg2[i][0];
		}
		setStringTokens(args[0],' ',pargs1);
		setStringTokens(args[1],' ',pargs2);
		int fd[2];
		pid_t pid1,pid2;
		if(pipe(fd) == 0)
		{
			if((pid1=fork()) == 0){
				//				close(fd[1]);
				if(dup2(fd[0],0)> -1){
		//			puts("sssss");
					close(fd[1]);
					execvp(pargs2[0],pargs2);
				}else{
					puts("Error creating dup2 for child\n");
					exit(0);
				//	return;
				}
				close(0);
				exit(1);
					//			close(0);
			}
			else if(pid1 > 0){
				if( (pid2=fork()) == 0){
					//					close(fd[0]);
					if(dup2(fd[1],1)> -1){
					//	puts("Success\n");
						close(fd[0]);
						execvp(pargs1[0],pargs1);
					}else{
						puts("Error Creating dup2 for parent\n");
						exit(0);
						//return;
					}
					close(1);
					exit(1);
					//					close(1);
				}
				else if(pid2 > 0){
//					puts("parent");
				//	wait(0);
					close(fd[0]);
					close(fd[1]);
					wait(0);
				}
				else{
					puts("Error");
				}

			}
			else{
				puts("Error in creating process");
			}
		}
	}	
}

int getInputArgCounts(){
	int i=0;
	while(args[i]!=NULL)
	{
		i=i+1;
	}
	return i;
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


void initargs(){
	for(int i=0;i<1000;i++){
		args[i] = &arg[i][0];
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
		args[i] = &arg[i][0];
		for(int j=0;j<1000;j++){
			arg[i][j] = '\0' ;		
		}
	}
}
void setvar(char *args[]){
	char pul[3][1000];//={"aaaaaaaaA","aaaaaaaaA"};
	char *a[3];
	a[0]=&pul[0][0];
	a[1]=&pul[1][0];
	a[2]=&pul[2][0];
	setStringTokens(args[1],'=',a);
	if(strcmp("PS1",a[0])==0){
		strcpy(prompt,a[1]);
	}
	if(strcmp("PATH",a[0])==0){
		strcpy(&env_var[env_length][0],a[1]);
		strcpy(path,a[1]);
		//  setenv("PATH", a[1], 1);
	}
}
void setStringTokens(char* str, char delimiter, char* strs[]){
	int i=0,j=0,k=0;
	while(str[k]!='\n'){
		if(str[k] == delimiter || str[k] == '\0'){
			strs[i][j]='\0';
			if(strcmp(strs[i],"&") == 0){
				isBackground = 1;
				strs[i] = NULL; 
			}
			if(str[k]=='\0'){	
				strs[i+1] = NULL; //Because execvp expects a NULL pointed args[] as the end
				return;	
			}
			i++;
			j=0;
		}
		else{
			strs[i][j++]=str[k];
		}
		++k;
	}	
}
