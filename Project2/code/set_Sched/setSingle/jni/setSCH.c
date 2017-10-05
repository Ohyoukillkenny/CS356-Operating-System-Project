#include <sched.h>
#include <stdio.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/unistd.h>
int main(int argc, char const *argv[])
{
	struct sched_param param;
	int myScheduler;
	printf("Please input the Scheduling method (0-normal,1-FIFO,2-RR): ");
	scanf("%d",&myScheduler); 
	switch(myScheduler)
	{
		case 0 : printf("Current scheduling method is SCHED_Normal\n");break;
		case 1 : printf("Current scheduling method is SCHED_FIFO\n");break;
		case 2 : printf("Current scheduling method is SCHED_RR\n");break;
		default: printf("Invalid input.\n"); return -1;break;
	}

	int myPri; 
	int processpid;
	printf("Please input the id of the testprocess : ");
	scanf("%d",&processpid);
	if (myScheduler == 0)
	{
		myPri = 0;
	}
	else
	{
		printf("Set Process's priority (1-99): ");
		scanf("%d", &myPri);
		if (myPri < 1 || myPri > 99)
		{
			perror("Priority level doesn't exist.");
			return -1;
		}
	}
	printf("current scheduler's priority is : ");
	printf("%d\n",myPri);
	printf("pre scheduler : ");
	printf("%d\n", sched_getscheduler(processpid));
	param.sched_priority = myPri;

	if (sched_setscheduler(processpid, myScheduler, &param) == -1) //设置优先级
	{ 
	        perror("sched_setscheduler() failed"); 
	        return -1;
	} 
	printf("cur scheduler : ");
	printf("%d\n", sched_getscheduler(processpid));
	return 0;
}

//com.osprj.test.processtest 
//pid processpid
