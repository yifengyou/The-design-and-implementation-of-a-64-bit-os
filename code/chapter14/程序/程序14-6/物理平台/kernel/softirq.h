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

#ifndef __SOFTIRQ_H__

#define __SOFTIRQ_H__

#define TIMER_SIRQ	(1 << 0)

extern unsigned long softirq_status;

struct softirq
{
	void (*action)(void * data);
	void * data;
};

extern struct softirq softirq_vector[64];


void register_softirq(int nr,void (*action)(void * data),void * data);
void unregister_softirq(int nr);

void set_softirq_status(unsigned long status);
unsigned long get_softirq_status();

void softirq_init();

#endif
