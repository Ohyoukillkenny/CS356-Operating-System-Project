#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/unistd.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/init_task.h>

MODULE_LICENSE("Dual BSD/GOL");
#define __NR_SYS_PTREE 356

#define _MAX_PROCESS 500


	    
struct prinfo {
	pid_t parent_pid;
	/* process id of parent */
	pid_t pid;
	/* process id */
	pid_t first_child_pid; /* pid of youngest child */
	pid_t next_sibling_pid; /* pid of older sibling */
	long state;
	long uid;
	/* current state of process */
	/* user id of process owner */
	char comm[64];
	/* name of program executed */
}Processes[_MAX_PROCESS];

//set the stack to instore the task_struct pointer
struct Stack{
	struct task_struct *data;
	struct Stack* below;
};


static int (*oldcall)(void);
static int sys_ptree(struct prinfo* buf, int* nr){

	// if visited[pid] == 1, means that we have visited this process in previous traverse
	bool visited[_MAX_PROCESS] = {0};
	// to tell whether a process have a  child
	bool exitChild = false;
	read_lock(&tasklist_lock);
	//cur points to the process that is being traversed 
	struct task_struct* cur = NULL;
	//following variables assists the program
	struct task_struct* child = NULL;
	struct task_struct* sib = NULL ;
	struct list_head* lh = NULL;


	int i = 0 ;
	//id of the process that is being traversed 
	int cur_id = 0;
	
	//top points to the element on the top of the stack, temp is assistant variable
	struct Stack* top = NULL;
	struct Stack* temp = NULL;

	//let cur points to the pid=0 process
	cur = &init_task;

	while(buf != NULL && nr != NULL){
		while(true){
			if(visited[cur->pid]==false){	

				visited[cur->pid] = true;
				//traverse:set the prinfo information
				Processes[cur_id].pid = cur->pid;
				Processes[cur_id].state = cur->state;
				Processes[cur_id].uid = cur->cred->uid;

				if(cur->pid == 0) Processes[cur_id].parent_pid = 0;
				else Processes[cur_id].parent_pid = cur->parent->pid;

				if(cur->sibling.next == cur->sibling.prev) Processes[cur_id].next_sibling_pid = 0;
				else{
					lh = cur->sibling.next; 
					sib = list_entry(lh, struct task_struct, sibling);
					Processes[cur_id].next_sibling_pid = sib->pid;                             			                                                            
				}
				

				if(cur->children.next == cur->children.prev) Processes[cur_id].first_child_pid = 0;
				else{
				 	lh = cur->children.next;
				 	child = list_entry(lh, struct task_struct, sibling);
				 	Processes[cur_id].first_child_pid = child->pid;	
				}

				for(i=0;cur->comm[i];i++){
					Processes[cur_id].comm[i] = cur->comm[i];
				}
				Processes[cur_id].comm[i] = cur->comm[i];				
				cur_id++;
			}

			//assume this process has no child at first, do traverse on its child
			//if exist a child,push into the stack
			exitChild = false;
			list_for_each(lh, &cur->children){
				child = list_entry(lh, struct task_struct, sibling);
				if(visited[child->pid]) continue;
				temp = kmalloc(sizeof(struct Stack), GFP_KERNEL);
				temp->below = top;
				temp->data = cur;
				top = temp;
				cur = child;
				exitChild = true;	
				break;
			}
			//if exist no child, go back
			if(exitChild == false) break; 
		}		
		//If stack is empty, we finished the traverse
		//If non-empty, pop the stack and traverse the poped element
		if(top == NULL) break;
		else{
			cur = top->data;
			temp = top;
			top = temp->below;
			kfree(temp);

			list_for_each(lh, &cur->children){
				child = list_entry(lh, struct task_struct, sibling);
				if(visited[child->pid]) continue;
				
				temp = kmalloc(sizeof(struct Stack), GFP_KERNEL);
				temp->below = top;
				temp->data = cur;
				top = temp;
				cur=child;


				break;
			}			
		}
	}	
	read_unlock(&tasklist_lock);
	//copy our result to user state!
	if(copy_to_user(buf, Processes, cur_id*sizeof(struct prinfo)))
		return 0;
	if(copy_to_user(nr, &cur_id , sizeof(int)))
		return 0;
	return 1;
}


static int addsyscall_init(void){
	long *syscall = (long*)0xc000d8c4;
	oldcall = (int(*)(void))(syscall[__NR_SYS_PTREE]);
	syscall[__NR_SYS_PTREE] = (unsigned long )sys_ptree;
	printk(KERN_INFO "module loead!\n");
	return 0;
}
static void addsyscall_exit(void){
	long *syscall = (long*)0xc000d8c4;
	syscall[__NR_SYS_PTREE] = (unsigned long )oldcall;
	printk(KERN_INFO "module exit!\n");
}

module_init(addsyscall_init);
module_exit(addsyscall_exit);
