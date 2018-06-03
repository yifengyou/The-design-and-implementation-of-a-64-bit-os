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

#ifndef __TIMER_H__

#define __TIMER_H__

#include "lib.h"

extern unsigned long volatile jiffies;

struct timer_list
{
	struct List list;
	unsigned long expire_jiffies;
	void (* func)(void * data);
	void *data;
};

extern struct timer_list timer_list_head;

void init_timer(struct timer_list * timer,void (* func)(void * data),void *data,unsigned long expire_jiffies);

void add_timer(struct timer_list * timer);

void del_timer(struct timer_list * timer);

void timer_init();

void do_timer();

#endif
