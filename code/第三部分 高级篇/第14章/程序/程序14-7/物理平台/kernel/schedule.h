/***************************************************
*		版权声明
*
*	本操作系统名为：MINE
*	该操作系统未经授权不得以盈利或非盈利为目的进行开发，
*	只允许个人学习以及公开交流使用
*
*	代码最终所有权及解释权归田宇所有；
*
*	本模块作者：	田宇
*	EMail:		345538255@qq.com
*
*
***************************************************/

#ifndef __SCHEDULE_H__

#define __SCHEDULE_H__

#include "task.h"

struct schedule
{
	long running_task_count;
	long CPU_exec_task_jiffies;
	struct task_struct task_queue;
};

extern struct schedule task_schedule[NR_CPUS];

void schedule();
void schedule_init();
struct task_struct *get_next_task();
void insert_task_queue(struct task_struct *tsk);

#endif
