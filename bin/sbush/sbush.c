#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
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
void setenvs();

static int isBackground = 0;
static char input[1025]={'\0'};
static char com[1025]={'\0'};
static char arg[1000][1000]={'\0'};
static char prompt[1000]={'\0'};
static char *in = &input[0] ;
static char *args[1000] ;
static char *command = &com[0];
static char *envpe[100];

int main(int argc, char *argv[], char *envp[]) {
    initargs();
    strcpy(prompt,"sbush");
    setenvs();
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
        int fp;
        fp = fopen(argv[1],"r");
        if(fp == -1){
            puts("File not found, Exiting!");
            return 0;
        }
        int c=1,i=0;
        while(c!=EOF){
            c = fgetc(fp);
            while(c!='\n' && c!=EOF){
                in[i++]=(char) c;
                //lseek(fd,1,SEEK_CUR);
                c = fgetc(fp);
            }
            if(i>0){
                parseInput();
                i=0;
                clearInput();
                clearCommand();
                clearArguments();
            }
            else{
                return 0;
            }
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
    if ( (pid = fork()) == 0) {
        if(execvp(cmd, ag,envpe) == -1){
            puts("-sbush: ");
            puts(cmd);
            puts(": No such command. \n");
        }
        exit(1);
    }
    else if (pid < 0) {
        puts("Failed to fork\n");
    }
    else {
        if(isBackground == 1){
            return;
        }
        waitpid(pid,&pid);
    }
}
void readInput(){
   in = &input[0];
   gets(in);
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
    for(int i =0;i<1025;i++) {
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
        setenv("PATH", a[1], 1);
        setenvs();
    }
}
void setStringTokens(char* str, char delimiter, char* strs[]){
    int i=0,j=0,k=0;
    while(str[k]!='\n'){
        if( k == 0 && str[k] == ' '){
            k++;
            continue;
        }
        else if(str[k] == delimiter || str[k] == '\0'){
            strs[i][j]='\0';
            if(strcmp(strs[i],"&") == 0){
                isBackground = 1;
                strs[i] = NULL;
            }
            if(str[k]=='\0'){
                if(j==0){
                    strs[i] = NULL;
                }
                strs[i+1] = NULL; //Because execvp expects a NULL pointed args[] as the end
                return;
            }
            i++;
            j=0;
        }
        else{
            if(str[k] == '"' && delimiter == '='){
                k++;
                continue;
            }
            strs[i][j++]=str[k];
        }
        ++k;
    }
}
void setenvs(){
    int i = getenvlength();
    for(int j=0;j<i;j++)
    {
        envpe[j] = getallenv(j);
    }
}
