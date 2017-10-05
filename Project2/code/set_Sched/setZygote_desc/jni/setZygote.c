#include <stdio.h>
#include <stdlib.h>             
#include <stdio.h>              
#include <sched.h>     
#include <unistd.h>

#define MAX_TASK_NUMBER 1000
struct prinfo {
	pid_t parent_pid;
	pid_t pid;
	pid_t first_child_pid; 
	pid_t next_sibling_pid;
	long state;
	long uid;
	char comm[64];
};


int find_descendants(pid_t ID, pid_t* queue, int num);
void add_descendants(pid_t ID,pid_t* queue, int num);
int main(){
	
	int num_pro;
    struct prinfo userPro[MAX_TASK_NUMBER];
    struct sched_param param;
    // set the priority 
    param.sched_priority = 10;
	syscall(356,userPro,&num_pro);

	int i = 0;
	int j;
	int num = 0;
	struct prinfo p;
	pid_t DES_pid[MAX_TASK_NUMBER]={0};

	for(i=0;i<num_pro;i++){
		p = userPro[i];
		if(p.comm[0]=='m'&&p.comm[1]=='a'&&p.comm[2]=='i'&&p.comm[3]=='n'){
			add_descendants(p.pid,DES_pid,num++);
			printf("zygote pid = %d\n",p.pid);
			for(j = 0; j < num; j++){
				printf("%d \n",DES_pid[j]);
				printf("num = %d",num);
			}
		}else if(find_descendants(p.parent_pid,DES_pid,num)){
			add_descendants(p.pid,DES_pid,num++);
			printf("descendants pid = %d\n",p.pid);
			if(sched_setscheduler(p.pid, SCHED_RR, &param) == -1){
			perror("set_SCHED error!\n");
			exit(1);			
			}
		}
	}		
	return 0;
}

int find_descendants(pid_t ID, pid_t* queue, int num){
	int i;
	for(i = 0; i<num ; i++)
	{
		if(queue[i] == ID)
		{
			return 1;
		}
	}
	return 0;
}

void add_descendants(pid_t ID,pid_t* queue, int num)
{
	int i;
	if(find_descendants(ID,queue,num))
	{
		return;
	}
	queue[num] = ID;
	return;
}

