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


void timer_init()
{
	jiffies = 0;
	register_softirq(0,&do_timer,NULL);
}

void do_timer(void * data)
{
	color_printk(RED,WHITE,"(HPET:%ld)",jiffies);
}
