#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h> 

//calculate n!
void calc_permulate(int n)
{
  int i = 0;
  volatile   long long  res  = 1;
	for(i = n; i > 0; i--)
	{
		res  = res * n;
	}
}


int main()
{
	struct timeval time_start,time_pend;
	float finalusage;
	int n = 0;
	int  getinput = 0;

	while(1){
	  
	  printf("Please input one large number (-1 to quit) :\n");
	  if(getinput == 0)  scanf("%d", &n);
	  if(n == -1) break;
	  if(n >= 1)
	  {
	        getinput = 1;
			gettimeofday(&time_start,0);
			calc_permulate(n);
			gettimeofday(&time_pend,0);
			finalusage = 1000000 * (time_pend.tv_sec-time_start.tv_sec) + time_pend.tv_usec - time_start.tv_usec ;
			finalusage /= 1000;
			printf("used time: %f ms\n",finalusage);
			//exit(0);
	    }
	    
	    n = 0;
	    getinput = 0;
	}

	return 0;
}
