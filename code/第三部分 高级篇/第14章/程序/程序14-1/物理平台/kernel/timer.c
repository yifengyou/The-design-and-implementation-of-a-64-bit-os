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

#include "timer.h"
#include "softirq.h"
#include "printk.h"
#include "lib.h"
#include "memory.h"

unsigned long volatile jiffies = 0;
struct timer_list timer_list_head;

void init_timer(struct timer_list * timer,void (* func)(void * data),void *data,unsigned long expire_jiffies)
{
	list_init(&timer->list);
	timer->func = func;
	timer->data = data;
	timer->expire_jiffies = jiffies + expire_jiffies;
}

void add_timer(struct timer_list * timer)
{
	struct timer_list * tmp = container_of(list_next(&timer_list_head.list),struct timer_list,list);

	if(!list_is_empty(&timer_list_head.list))
	{
		
	}
	else
	{
		while(tmp->expire_jiffies < timer->expire_jiffies)
			tmp = container_of(list_next(&tmp->list),struct timer_list,list);
	}

	list_add_to_behind(&tmp->list,&timer->list);
}

void del_timer(struct timer_list * timer)
{
	list_del(&timer->list);
}

void test_timer(void * data)
{
	color_printk(BLUE,WHITE,"test_timer");
}

void timer_init()
{
	struct timer_list *tmp = NULL;
	jiffies = 0;
	init_timer(&timer_list_head,NULL,NULL,-1UL);
	register_softirq(0,&do_timer,NULL);

	tmp = (struct timer_list *)kmalloc(sizeof(struct timer_list),0);
	init_timer(tmp,&test_timer,NULL,5);
	add_timer(tmp);
}

void do_timer(void * data)
{
	struct timer_list * tmp = container_of(list_next(&timer_list_head.list),struct timer_list,list);
	while((!list_is_empty(&timer_list_head.list)) && (tmp->expire_jiffies <= jiffies))
	{
		del_timer(tmp);
		tmp->func(tmp->data);
		tmp = container_of(list_next(&timer_list_head.list),struct timer_list,list);
	}

	color_printk(RED,WHITE,"(HPET:%ld)",jiffies);
}
