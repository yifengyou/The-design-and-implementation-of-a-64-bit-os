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
#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__
#include "linkage.h"
#include "ptrace.h"

/*

*/

typedef struct hw_int_type
{
	void (*enable)(unsigned long irq);
	void (*disable)(unsigned long irq);

	unsigned long (*install)(unsigned long irq,void * arg);
	void (*uninstall)(unsigned long irq);

	void (*ack)(unsigned long irq);
}hw_int_controller;

/*

*/

typedef struct {
	hw_int_controller * controller;

	char * irq_name;
	unsigned long parameter;
	void (*handler)(unsigned long nr, unsigned long parameter, struct pt_regs * regs);
	unsigned long flags;
}irq_desc_T;

/*

*/

#define NR_IRQS 24

irq_desc_T interrupt_desc[NR_IRQS] = {0};

/*

*/

int register_irq(unsigned long irq,
		void * arg,
		void (*handler)(unsigned long nr, unsigned long parameter, struct pt_regs * regs),
		unsigned long parameter,
		hw_int_controller * controller,
		char * irq_name);

/*

*/

int unregister_irq(unsigned long irq);

extern void (* interrupt[24])(void);

extern void do_IRQ(struct pt_regs * regs,unsigned long nr);

#endif
