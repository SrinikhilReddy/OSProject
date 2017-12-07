#include <sys/defs.h>
#include <string.h>
#include <unistd.h>
int strtoInt(char* num){
    int dec = 0, i, len;
    len = strlen(num);
    for(i=0; i<len; i++){
        dec = dec * 10 + ( num[i] - '0' );
    }
    return dec;
}
int main(int argc, char* argv[], char* envp[])
{
    int k = strtoInt(argv[1]);
    sleep(k);
    return 0;
}
