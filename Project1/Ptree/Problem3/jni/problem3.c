#include "prinfo.h"
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <malloc.h>

int main(int argc, char const *argv[])
{
	pid_t fpid; //fpid means the ret result of the fork()
    fpid=fork();   
    if (fpid < 0)   
    {
        perror("Error:");
        return -1; 
    } 
    else if (fpid == 0) 
    {  
        printf("5140219016Child : %d\n",getpid());
        execl("/data/mycode/ptreeARM","./ptreeARM",NULL); 
        int test;  
        if ((test = execl("/data/mycode/ptree_ARM","./ptree_ARM",NULL))== -1)
        {
        	perror("Error:");
        	return -1;
        }
    }  
    else {  
        printf("5140219016Parent: %d\n",getpid()); 
        sleep(5);  //avoid parent being killed, make sure that we can see parent in the final ptree
    }  
    return 0;
}
      
