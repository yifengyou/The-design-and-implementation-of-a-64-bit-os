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

#ifndef __SEMAPHORE_H__

#define __SEMAPHORE_H__

#include "atomic.h"
#include "lib.h"
#include "task.h"
#include "schedule.h"

typedef struct
{
	struct List wait_list;
	struct task_struct *tsk;
} wait_queue_T;

void wait_queue_init(wait_queue_T * wait_queue,struct task_struct *tsk)
{
	list_init(&wait_queue->wait_list);
	wait_queue->tsk = tsk;
}

typedef struct 
{
	atomic_T counter;
	wait_queue_T wait;
} semaphore_T;

void semaphore_init(semaphore_T * semaphore,unsigned long count)
{
	atomic_set(&semaphore->counter,count);
	wait_queue_init(&semaphore->wait,NULL);
}

void __up(semaphore_T * semaphore)
{
	wait_queue_T * wait = container_of(list_next(&semaphore->wait.wait_list),wait_queue_T,wait_list);

	list_del(&wait->wait_list);
	wait->tsk->state = TASK_RUNNING;
	insert_task_queue(wait->tsk);
}

void semaphore_up(semaphore_T * semaphore)
{
	if(list_is_empty(&semaphore->wait.wait_list))
		atomic_inc(&semaphore->counter);
	else
		__up(semaphore);
}

void __down(semaphore_T * semaphore)
{
	wait_queue_T wait;
	wait_queue_init(&wait,current);
	current->state = TASK_UNINTERRUPTIBLE;
	list_add_to_before(&semaphore->wait.wait_list,&wait.wait_list);

	schedule();
}

void semaphore_down(semaphore_T * semaphore)
{
	if(atomic_read(&semaphore->counter) > 0)
		atomic_dec(&semaphore->counter);
	else
		__down(semaphore);
}

#endif
