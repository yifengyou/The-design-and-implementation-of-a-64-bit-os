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

#ifndef __TASK_H__

#define __TASK_H__

#include "memory.h"
#include "cpu.h"
#include "lib.h"
#include "ptrace.h"
#include "VFS.h"
#include "waitqueue.h"

#define KERNEL_CS 	(0x08)
#define	KERNEL_DS 	(0x10)

#define	USER_CS		(0x28)
#define USER_DS		(0x30)

// stack size 32K
#define STACK_SIZE 32768

extern char _text;
extern char _etext;
extern char _data;
extern char _edata;
extern char _rodata;
extern char _erodata;
extern char _bss;
extern char _ebss;
extern char _end;

extern unsigned long _stack_start;
extern long global_pid;

/*

*/

#define TASK_RUNNING		(1 << 0)
#define TASK_INTERRUPTIBLE	(1 << 1)
#define	TASK_UNINTERRUPTIBLE	(1 << 2)
#define	TASK_ZOMBIE		(1 << 3)	
#define	TASK_STOPPED		(1 << 4)

/*

*/


struct mm_struct
{
	pml4t_t *pgd;	//page table point
	
	unsigned long start_code,end_code;
	unsigned long start_data,end_data;
	unsigned long start_rodata,end_rodata;
	unsigned long start_bss,end_bss;
	unsigned long start_brk,end_brk;
	unsigned long start_stack;
};

/*

*/

struct thread_struct
{
	unsigned long rsp0;	//in tss

	unsigned long rip;
	unsigned long rsp;	

	unsigned long fs;
	unsigned long gs;

	unsigned long cr2;
	unsigned long trap_nr;
	unsigned long error_code;
};

/*

*/

#define TASK_FILE_MAX	10

struct task_struct
{
	volatile long state;
	unsigned long flags;
	long preempt_count;
	long signal;
	long cpu_id;		//CPU ID

	struct mm_struct *mm;
	struct thread_struct *thread;

	struct List list;

	unsigned long addr_limit;	/*0x0000,0000,0000,0000 - 0x0000,7fff,ffff,ffff user*/
					/*0xffff,8000,0000,0000 - 0xffff,ffff,ffff,ffff kernel*/
	long pid;
	long priority;
	long vrun_time;

	long exit_code;

	struct file * file_struct[TASK_FILE_MAX];

	wait_queue_T wait_childexit;
	struct task_struct *next;
	struct task_struct *parent;
};

///////struct task_struct->flags:

#define PF_KTHREAD	(1UL << 0)
#define NEED_SCHEDULE	(1UL << 1)
#define PF_VFORK	(1UL << 2)


union task_union
{
	struct task_struct task;
	unsigned long stack[STACK_SIZE / sizeof(unsigned long)];
}__attribute__((aligned (8)));	//8Bytes align



#define INIT_TASK(tsk)	\
{			\
	.state = TASK_UNINTERRUPTIBLE,		\
	.flags = PF_KTHREAD,		\
	.preempt_count = 0,		\
	.signal = 0,		\
	.cpu_id = 0,		\
	.mm = &init_mm,			\
	.thread = &init_thread,		\
	.addr_limit = 0xffffffffffffffff,	\
	.pid = 0,			\
	.priority = 2,		\
	.vrun_time = 0,		\
	.exit_code = 0,		\
	.file_struct = {0},	\
	.next = &tsk,		\
	.parent = &tsk,		\
}

/*

*/

struct tss_struct
{
	unsigned int  reserved0;
	unsigned long rsp0;
	unsigned long rsp1;
	unsigned long rsp2;
	unsigned long reserved1;
	unsigned long ist1;
	unsigned long ist2;
	unsigned long ist3;
	unsigned long ist4;
	unsigned long ist5;
	unsigned long ist6;
	unsigned long ist7;
	unsigned long reserved2;
	unsigned short reserved3;
	unsigned short iomapbaseaddr;
}__attribute__((packed));

#define INIT_TSS \
{	.reserved0 = 0,	 \
	.rsp0 = (unsigned long)(init_task_union.stack + STACK_SIZE / sizeof(unsigned long)),	\
	.rsp1 = (unsigned long)(init_task_union.stack + STACK_SIZE / sizeof(unsigned long)),	\
	.rsp2 = (unsigned long)(init_task_union.stack + STACK_SIZE / sizeof(unsigned long)),	\
	.reserved1 = 0,	 \
	.ist1 = 0xffff800000007c00,	\
	.ist2 = 0xffff800000007c00,	\
	.ist3 = 0xffff800000007c00,	\
	.ist4 = 0xffff800000007c00,	\
	.ist5 = 0xffff800000007c00,	\
	.ist6 = 0xffff800000007c00,	\
	.ist7 = 0xffff800000007c00,	\
	.reserved2 = 0,	\
	.reserved3 = 0,	\
	.iomapbaseaddr = 0	\
}


/*

*/


inline	struct task_struct * get_current()
{
	struct task_struct * current = NULL;
	__asm__ __volatile__ ("andq %%rsp,%0	\n\t":"=r"(current):"0"(~32767UL));
	return current;
}

#define current get_current()

#define GET_CURRENT			\
	"movq	%rsp,	%rbx	\n\t"	\
	"andq	$-32768,%rbx	\n\t"

/*

*/


#define switch_to(prev,next)			\
do{							\
	__asm__ __volatile__ (	"pushq	%%rbp	\n\t"	\
				"pushq	%%rax	\n\t"	\
				"movq	%%rsp,	%0	\n\t"	\
				"movq	%2,	%%rsp	\n\t"	\
				"leaq	1f(%%rip),	%%rax	\n\t"	\
				"movq	%%rax,	%1	\n\t"	\
				"pushq	%3		\n\t"	\
				"jmp	__switch_to	\n\t"	\
				"1:	\n\t"	\
				"popq	%%rax	\n\t"	\
				"popq	%%rbp	\n\t"	\
				:"=m"(prev->thread->rsp),"=m"(prev->thread->rip)		\
				:"m"(next->thread->rsp),"m"(next->thread->rip),"D"(prev),"S"(next)	\
				:"memory"		\
				);			\
}while(0)

/*

*/
long get_pid();
struct task_struct *get_task(long pid);

inline void wakeup_process(struct task_struct *tsk);
void exit_files(struct task_struct *tsk);

unsigned long do_fork(struct pt_regs * regs, unsigned long clone_flags, unsigned long stack_start, unsigned long stack_size);
unsigned long do_execve(struct pt_regs *regs,char *name,char *argv[],char *envp[]);
unsigned long do_exit(unsigned long exit_code);

void task_init();

inline void switch_mm(struct task_struct *prev,struct task_struct *next);

extern void ret_system_call(void);
extern void system_call(void);

extern struct task_struct *init_task[NR_CPUS];
extern union task_union init_task_union;
extern struct mm_struct init_mm;
extern struct thread_struct init_thread;

extern struct tss_struct init_tss[NR_CPUS];

#endif
