#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char env_var[1000][1000];
int env_length=0;
char temp[50];
char* getvar(char* name);

char *getenv(const char *name){
	for(int i=0;i<env_length;i++)
	{
		getvar(&env_var[i][0]);
		int k = strlen(&temp[0]);
		return &env_var[i][k];
	}
	return NULL;	
}

int setenv( char *name, char *value, int overwrite){
	for(int i=0;i<env_length;i++){
		getvar(&env_var[i][0]);
		if(strcmp(name,&temp[0])== 0){
			int k = strlen(&temp[0]);
			strcpy(&env_var[i][k],value);
			return 1;
		}
	}
	return 0;
}

void pushenvs(char* envp[]){
	int i=0;
	while(envp[i]!=NULL)
        {   
                strcpy(&env_var[i][0],envp[i]);
                i++;
        }
	env_length=i;  
}
char* getvar(char* name){
	int i =0;
	while(*name!='='){
		temp[i++] = *name;
		name++;	
	}
	temp[i] = '\0';
	return &temp[0];
}
