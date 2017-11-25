#include<sys/process.h>
#include<sys/mem.h>

struct task_struct q[MAX];//running;
int np=0;

void addToQ(task_struct a){
	q[np++] = a;
}
