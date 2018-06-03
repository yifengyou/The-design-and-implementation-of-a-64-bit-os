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

#ifndef __SPINLOCK_H__

#define __SPINLOCK_H__

#include "preempt.h"
#include "lib.h"

/*

*/

typedef struct
{
	__volatile__ unsigned long lock;		//1:unlock,0:lock
}spinlock_T;

/*

*/

inline void spin_init(spinlock_T * lock)
{
	lock->lock = 1;
}

/*

*/

inline void spin_lock(spinlock_T * lock)
{
	preempt_disable();
	__asm__	__volatile__	(	"1:	\n\t"
					"lock	decq	%0	\n\t"
					"jns	3f	\n\t"
					"2:	\n\t"
					"pause	\n\t"
					"cmpq	$0,	%0	\n\t"
					"jle	2b	\n\t"
					"jmp	1b	\n\t"
					"3:	\n\t"
					:"=m"(lock->lock)
					:
					:"memory"
				);
}

/*

*/

inline void spin_unlock(spinlock_T * lock)
{
	__asm__	__volatile__	(	"movq	$1,	%0	\n\t"
					:"=m"(lock->lock)
					:
					:"memory"
				);
	preempt_enable();
}

inline long spin_trylock(spinlock_T * lock)
{
	unsigned long tmp_value = 0;
	preempt_disable();
	__asm__	__volatile__	(	"xchgq	%0,	%1	\n\t"
				:"=q"(tmp_value),"=m"(lock->lock)
				:"0"(0)
				:"memory"
			);
	if(!tmp_value)
		preempt_enable();
	return tmp_value;
}

#define local_irq_save(x)	__asm__ __volatile__("pushfq ; popq %0 ; cli":"=g"(x)::"memory")
#define local_irq_restore(x)	__asm__ __volatile__("pushq %0 ; popfq"::"g"(x):"memory")
#define local_irq_disable()	cli();
#define local_irq_enable()	sti();

#define spin_lock_irqsave(lock,flags)	\
do					\
{					\
	local_irq_save(flags);		\
	spin_lock(lock);		\
}while(0)

#define spin_unlock_irqrestore(lock,flags)	\
do						\
{						\
	spin_unlock(lock);			\
	local_irq_restore(flags);		\
}while(0)

#define spin_lock_irq(lock)	\
do				\
{				\
	local_irq_disable();	\
	spin_lock(lock);	\
}while(0)

#define spin_unlock_irq(lock)	\
do				\
{				\
	spin_unlock(lock);	\
	local_irq_enable();	\
}while(0)

#endif
