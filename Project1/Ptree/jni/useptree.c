#include "prinfo.h"
struct prinfo *Mystack[500];
int level = 0;

void level_push(struct prinfo *push)
{
	Mystack[level++] = push;
}
int pop_indent(int parent_pid)
{
    int i;
	for (i=level; i >0; i--)
	{
		if (Mystack[i-1]->pid == parent_pid)  break;
	}
    level = i;
	return level;
}
int main()
{
  	int nr = 500;	
	struct prinfo *p = malloc(nr * sizeof(struct prinfo)); //allocate the space for the prinfo
	int len = syscall(356, &p[0], &nr);
    int i,j;
	if (len < 0) 
	{
		perror("Error:");
		return -1;
	}
	for ( i = 0; i < len; i++) 
	{
		for ( j = 0; j < pop_indent(p[i].parent_pid); j++)  printf("\t");	  
	    level_push(&p[i]);
	    printf("%s,%d,%ld,%d,%d,%d,%ld\n", p[i].comm, p[i].pid,p[i].state, 
	    	p[i].parent_pid,p[i].first_child_pid,p[i].next_sibling_pid, p[i].uid);
	}
  
	return 0;
}