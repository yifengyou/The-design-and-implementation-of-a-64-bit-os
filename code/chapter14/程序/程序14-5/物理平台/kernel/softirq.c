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

#include "softirq.h"
#include "lib.h"

unsigned long softirq_status = 0;
struct softirq softirq_vector[64] = {0};

void set_softirq_status(unsigned long status)
{
	softirq_status |= status;
}

unsigned long get_softirq_status()
{
	return softirq_status;
}

void register_softirq(int nr,void (*action)(void * data),void * data)
{
	softirq_vector[nr].action = action;
	softirq_vector[nr].data = data;
}

void unregister_softirq(int nr)
{
	softirq_vector[nr].action = NULL;
	softirq_vector[nr].data = NULL;
}

void do_softirq()
{
	int i;
	sti();
	for(i = 0;i < 64 && softirq_status;i++)
	{
		if(softirq_status & (1 << i))
		{
			softirq_vector[i].action(softirq_vector[i].data);
			softirq_status &= ~(1 << i);
		}
	}
	cli();
}

void softirq_init()
{
	softirq_status = 0;
	memset(softirq_vector,0,sizeof(struct softirq) * 64);
}
