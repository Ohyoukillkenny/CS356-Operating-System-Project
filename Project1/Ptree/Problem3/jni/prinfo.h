#ifndef PRINFO_H_
#define prinfo_H_
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/unistd.h>
#include <stdio.h>
#include <stdlib.h>
struct prinfo {
    pid_t parent_pid;       /* process id of parent, set 0 if it has no parent*/ 
    pid_t pid;              /* process id */
    pid_t first_child_pid;  /* pid of youngest child, set 0 if it has no child */
    pid_t next_sibling_pid; /* pid of older sibling, set 0 if it has no sibling*/
    long state;             /* name of program executed */
    long uid;               /* current state of process */
    char comm[64];          /* user id of process owner */
 };
#endif
