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

#include "SMP.h"
#include "printk.h"
#include "lib.h"
#include "gate.h"
#include "spinlock.h"
#include "interrupt.h"
#include "task.h"
#include "schedule.h"

spinlock_T SMP_lock;
int global_i = 0;


void IPI_0x200(unsigned long nr, unsigned long parameter, struct pt_regs * regs)
{
	switch(current->priority)
	{
		case 0:
		case 1:
			task_schedule[SMP_cpu_id()].CPU_exec_task_jiffies--;
			current->vrun_time += 1;
			break;
		case 2:
		default:
			task_schedule[SMP_cpu_id()].CPU_exec_task_jiffies -= 2;
			current->vrun_time += 2;
			break;
	}
	if(task_schedule[SMP_cpu_id()].CPU_exec_task_jiffies <= 0)
		current->flags |= NEED_SCHEDULE;
}

void SMP_init()
{
	int i;
	unsigned int a,b,c,d;

	//get local APIC ID
	for(i = 0;;i++)
	{
		get_cpuid(0xb,i,&a,&b,&c,&d);
		if((c >> 8 & 0xff) == 0)
			break;
		color_printk(WHITE,BLACK,"local APIC ID Package_../Core_2/SMT_1,type(%x) Width:%#010x,num of logical processor(%x)\n",c >> 8 & 0xff,a & 0x1f,b & 0xff);
	}
	
	color_printk(WHITE,BLACK,"x2APIC ID level:%#010x\tx2APIC ID the current logical processor:%#010x\n",c & 0xff,d);
	
	color_printk(WHITE,BLACK,"SMP copy byte:%#010x\n",(unsigned long)&_APU_boot_end - (unsigned long)&_APU_boot_start);
	memcpy(_APU_boot_start,(unsigned char *)0xffff800000020000,(unsigned long)&_APU_boot_end - (unsigned long)&_APU_boot_start);

	spin_init(&SMP_lock);

	for(i = 200;i < 210;i++)
	{
		set_intr_gate(i , 0 , SMP_interrupt[i - 200]);
	}
	memset(SMP_IPI_desc,0,sizeof(irq_desc_T) * 10);
	
//	register_IPI(200,NULL,&IPI_0x200,NULL,NULL,"IPI 0x200");
}


void Start_SMP()
{
	unsigned int x,y;

	color_printk(RED,YELLOW,"APU starting......\n");

	//enable xAPIC & x2APIC
	__asm__ __volatile__(	"movq 	$0x1b,	%%rcx	\n\t"
				"rdmsr	\n\t"
				"bts	$10,	%%rax	\n\t"
				"bts	$11,	%%rax	\n\t"
				"wrmsr	\n\t"
				"movq 	$0x1b,	%%rcx	\n\t"
				"rdmsr	\n\t"
				:"=a"(x),"=d"(y)
				:
				:"memory");
	
	if(x&0xc00)
		color_printk(RED,YELLOW,"xAPIC & x2APIC enabled\n");

	//enable SVR[8] SVR[12]
	__asm__ __volatile__(	"movq 	$0x80f,	%%rcx	\n\t"
				"rdmsr	\n\t"
				"bts	$8,	%%rax	\n\t"
				"bts	$12,	%%rax\n\t"
				"wrmsr	\n\t"
				"movq 	$0x80f,	%%rcx	\n\t"
				"rdmsr	\n\t"
				:"=a"(x),"=d"(y)
				:
				:"memory");

	if(x&0x100)
		color_printk(RED,YELLOW,"SVR[8] enabled\n");
	if(x&0x1000)
		color_printk(RED,YELLOW,"SVR[12] enabled\n");

	//get local APIC ID
	__asm__ __volatile__(	"movq $0x802,	%%rcx	\n\t"
				"rdmsr	\n\t"
				:"=a"(x),"=d"(y)
				:
				:"memory");
	
	color_printk(RED,YELLOW,"x2APIC ID:%#010x\t",x);

	current->pid = global_pid++;
	current->state = TASK_RUNNING;
	current->flags = PF_KTHREAD;
	current->next = init_task_union.task.next;
	init_task_union.task.next = current;
	current->parent = &init_task_union.task;
	current->mm = &init_mm;

	list_init(&current->list);
	current->addr_limit = 0xffff800000000000;
	current->priority = 2;
	current->vrun_time = 0;

	current->thread = (struct thread_struct *)(current + 1);
	memset(current->thread,0,sizeof(struct thread_struct));
	current->thread->rsp0 = _stack_start;
	current->thread->rsp = _stack_start;
	current->thread->fs = KERNEL_DS;
	current->thread->gs = KERNEL_DS;
	init_task[SMP_cpu_id()] = current;

	load_TR(10 + (global_i -1) * 2);

	spin_unlock(&SMP_lock);
	current->preempt_count = 0;

	sti();

//	if(SMP_cpu_id() == 3)
//		task_init();

	while(1)
		hlt();
}
