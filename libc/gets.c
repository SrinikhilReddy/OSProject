#include "../include/syscall.h"
#include "../include/CONSTANTS.h"

int gets(char *string)
{
	fgets(string,stdin);
}
