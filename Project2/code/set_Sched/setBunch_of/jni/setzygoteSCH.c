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
	// input the id of processes which we want to operate
	int i = 0;
	int processpid[100];
	while(1)
	{
		printf("Please input the id of the testprocess (quit by -1) : ");
		scanf("%d",&processpid[i]);
		if (processpid[i]==-1)
		{
			break;
		}
		i ++ ;
	}
	// set Priority
	if (myScheduler == 0) //  sched_get_priority_max(0) only can be zero
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
	printf("%d\n", sched_getscheduler(processpid[0]));
	param.sched_priority = myPri;
	for (i = i - 1; i >= 0; --i) //exclude the -1
	{
		if (sched_setscheduler(processpid[i], myScheduler, &param) == -1) //设置优先级
		{ 
	        perror("sched_setscheduler() failed"); 
	        return -1;
		} 
	}
	
	printf("cur scheduler : ");
	printf("%d\n", sched_getscheduler(processpid[0]));
	return 0;
}

//com.osprj.test.processtest 
//pid processpid
