#include<sys/process.h>
#include<sys/mem.h>

struct task_struct q[100];//running;
int np=0;

void addToQ(task_struct a){
	q[np++] = a;
}
