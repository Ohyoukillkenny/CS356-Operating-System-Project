#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>  //the task_struct is in the sched.h
#include <linux/unistd.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include <linux/slab.h>

MODULE_LICENSE("Dual BSD/GPL");

#define __NR_ptree 356
static int (*oldcall)(void);

struct prinfo {
    pid_t parent_pid;       /* process id of parent, set 0 if it has no parent*/ 
    pid_t pid;              /* process id */
    pid_t first_child_pid;  /* pid of youngest child, set 0 if it has no child */
    pid_t next_sibling_pid; /* pid of older sibling, set 0 if it has no sibling*/
    long state;             /* name of program executed */
    long uid;               /* current state of process */
    char comm[64];          /* user id of process owner */
 };

void copy(struct task_struct* ptr , struct prinfo* buf)
{
    struct task_struct* p_cptr; // youngest child
    struct task_struct* p_osptr;// older sibling
    buf->pid = ptr->pid;     //pid

    if (ptr->parent == ptr)
    {
      buf->parent_pid = 0;
    }
    else
    {
      buf->parent_pid = ptr->parent->pid;  //parent's pid
    }

    strcpy(buf->comm , ptr->comm);    // name
    buf->state = ptr->state;  // state
    buf->uid = ptr->cred->uid;      // uid

    if (list_empty(&ptr->children))
    {
      buf->first_child_pid = 0;
    }
    else{
      p_cptr = list_entry(ptr->children.next, struct task_struct, sibling); //youngest child
      buf->first_child_pid =p_cptr -> pid;
    }
    
     // older brother
    if (ptr->parent == ptr)
    {
      buf->next_sibling_pid = 0;
    }
    else 
    {
      if (list_is_last(&ptr->sibling, &ptr->parent->children))
      {
        buf->next_sibling_pid = 0;
      }
      else{
        p_osptr = list_entry(ptr->sibling.next, struct task_struct, sibling);
        buf->next_sibling_pid = p_osptr -> pid;
      }
    }
}
int dfs(struct task_struct* ptr,struct prinfo* klk,int nr)
{
  int num = 0;
  struct task_struct* ps = ptr;
  while (1)
 {
  if (thread_group_leader(ps))
  {
    if (num < nr)
    {
      copy(ps,klk);
      klk++;
    }
    if (++num == nr)
    {
      return num;
    }
  }

  if (!list_empty(&ps->children)) 
  {
    ps = list_entry(ps->children.next, struct task_struct, sibling); // has children
    continue;
  }
  if (!list_is_last(&ps->sibling, &ps->parent->children)) 
  {
    ps = list_entry(ps->sibling.next, struct task_struct, sibling); // has sibling
    continue;
  }    
  //find process that has not been visited
  while (ps->parent->parent != ps->parent && list_is_last(&ps->parent->sibling, &ps->parent->parent->children)) 
  {
    ps = ps->parent;
  }    
  if (ps->parent->parent == ps->parent)  
  return num; 
  else ps = list_entry(ps->parent->sibling.next, struct task_struct, sibling);
  
  }
  return num;
}

static int sys_ptree(struct prinfo *buf, int *nr)
{  
  struct prinfo *klk,*klk_head; //klk: process
  int begin,num = 0;
  unsigned long bufsize;
  struct task_struct *task;

  read_lock(&tasklist_lock);
  task = &init_task;
  while (task->parent != task) 
    task = task->parent;      // make sure task is root
  read_unlock(&tasklist_lock);
  
  if (!buf || *nr < 1) // illegal value
    return -EINVAL;
//copy to user
  if (copy_from_user(&begin, nr, sizeof(int))) // No sound parameter
    return -EFAULT; //errno: 14

  bufsize = begin * sizeof(struct prinfo);
  klk = kmalloc(bufsize, GFP_KERNEL);
    
  klk_head = klk;

  read_lock(&tasklist_lock);
  num = dfs(task, klk, begin);
  read_unlock(&tasklist_lock);
  
  if (copy_to_user(buf, klk_head, bufsize))
    return -EFAULT;
  if (num < begin)
  if (copy_to_user(nr, &num, sizeof(int)))
    return -EFAULT;
  kfree(klk);

  return num;
}

static int addsyscall_init(void){
  long *syscall = (long*)0xc000d8c4;
  oldcall = (int(*)(void))(syscall[__NR_ptree]);
  syscall[__NR_ptree] = (unsigned long) sys_ptree;
  printk(KERN_INFO "module load!hhh\n");
  return 0;
}

static int addsyscall_exit(void){
  long *syscall = (long*)0xc000d8c4;
  syscall[__NR_ptree] = (unsigned long) oldcall;
  printk(KERN_INFO "module exit! orz \n");
}

module_init(addsyscall_init);
module_exit(addsyscall_exit);


