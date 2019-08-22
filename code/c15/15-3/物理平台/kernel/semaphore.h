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
#include "waitqueue.h"

typedef struct 
{
	atomic_T counter;
	wait_queue_T wait;
} semaphore_T;

void semaphore_init(semaphore_T * semaphore,unsigned long count);

void semaphore_up(semaphore_T * semaphore);

void semaphore_down(semaphore_T * semaphore);

#endif
