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

#include "task.h"
#include "ptrace.h"
#include "lib.h"
#include "memory.h"
#include "linkage.h"
#include "gate.h"
#include "schedule.h"
#include "printk.h"


struct mm_struct init_mm = {0};

struct thread_struct init_thread = 
{
	.rsp0 = (unsigned long)(init_task_union.stack + STACK_SIZE / sizeof(unsigned long)),
	.rsp = (unsigned long)(init_task_union.stack + STACK_SIZE / sizeof(unsigned long)),
	.fs = KERNEL_DS,
	.gs = KERNEL_DS,
	.cr2 = 0,
	.trap_nr = 0,
	.error_code = 0
};


union task_union init_task_union __attribute__((__section__ (".data.init_task"))) = {INIT_TASK(init_task_union.task)};

struct task_struct *init_task[NR_CPUS] = {&init_task_union.task,0};


struct tss_struct init_tss[NR_CPUS] = { [0 ... NR_CPUS-1] = INIT_TSS };

system_call_t system_call_table[MAX_SYSTEM_CALL_NR] = 
{
	[0] = no_system_call,
	[1] = sys_printf,
	[2 ... MAX_SYSTEM_CALL_NR-1] = no_system_call
};

unsigned long no_system_call(struct pt_regs * regs)
{
	color_printk(RED,BLACK,"no_system_call is calling,NR:%#04x\n",regs->rax);
	return -1;
}

unsigned long sys_printf(struct pt_regs * regs)
{
	color_printk(BLACK,WHITE,(char *)regs->rdi);
	return 1;
}

void user_level_function()
{
	long ret = 0;
//	color_printk(RED,BLACK,"user_level_function task is running\n");
	char string[]="Hello World!\n";

	__asm__	__volatile__	(	"leaq	sysexit_return_address(%%rip),	%%rdx	\n\t"
					"movq	%%rsp,	%%rcx		\n\t"
					"sysenter			\n\t"
					"sysexit_return_address:	\n\t"
					:"=a"(ret):"0"(1),"D"(string):"memory");	

//	color_printk(RED,BLACK,"user_level_function task called sysenter,ret:%ld\n",ret);

	while(1);
}


unsigned long do_execve(struct pt_regs * regs)
{
	unsigned long addr = 0x800000;
	unsigned long * tmp;
	unsigned long * virtual = NULL;
	struct Page * p = NULL;
	
	regs->rdx = 0x800000;	//RIP
	regs->rcx = 0xa00000;	//RSP
	regs->rax = 1;	
	regs->ds = 0;
	regs->es = 0;
	color_printk(RED,BLACK,"do_execve task is running\n");

	Global_CR3 = Get_gdt();

	tmp = Phy_To_Virt((unsigned long *)((unsigned long)Global_CR3 & (~ 0xfffUL)) + ((addr >> PAGE_GDT_SHIFT) & 0x1ff));

	virtual = kmalloc(PAGE_4K_SIZE,0);
	set_mpl4t(tmp,mk_mpl4t(Virt_To_Phy(virtual),PAGE_USER_GDT));

	tmp = Phy_To_Virt((unsigned long *)(*tmp & (~ 0xfffUL)) + ((addr >> PAGE_1G_SHIFT) & 0x1ff));
	virtual = kmalloc(PAGE_4K_SIZE,0);
	set_pdpt(tmp,mk_pdpt(Virt_To_Phy(virtual),PAGE_USER_Dir));

	tmp = Phy_To_Virt((unsigned long *)(*tmp & (~ 0xfffUL)) + ((addr >> PAGE_2M_SHIFT) & 0x1ff));
	p = alloc_pages(ZONE_NORMAL,1,PG_PTable_Maped);
	set_pdt(tmp,mk_pdt(p->PHY_address,PAGE_USER_Page));

	flush_tlb();
	
	if(!(current->flags & PF_KTHREAD))
		current->addr_limit = 0xffff800000000000;

	memcpy(user_level_function,(void *)0x800000,1024);

	return 1;
}


unsigned long init(unsigned long arg)
{
	struct pt_regs *regs;

	color_printk(RED,BLACK,"init task is running,arg:%#018lx\n",arg);

	current->thread->rip = (unsigned long)ret_system_call;
	current->thread->rsp = (unsigned long)current + STACK_SIZE - sizeof(struct pt_regs);
	regs = (struct pt_regs *)current->thread->rsp;
	current->flags = 0;

	__asm__	__volatile__	(	"movq	%1,	%%rsp	\n\t"
					"pushq	%2		\n\t"
					"jmp	do_execve	\n\t"
					::"D"(regs),"m"(current->thread->rsp),"m"(current->thread->rip):"memory");

	return 1;
}



unsigned long do_fork(struct pt_regs * regs, unsigned long clone_flags, unsigned long stack_start, unsigned long stack_size)
{
	struct task_struct *tsk = NULL;
	struct thread_struct *thd = NULL;
	struct Page *p = NULL;
	
	color_printk(WHITE,BLACK,"alloc_pages,bitmap:%#018lx\n",*memory_management_struct.bits_map);

	p = alloc_pages(ZONE_NORMAL,1,PG_PTable_Maped | PG_Kernel);

	color_printk(WHITE,BLACK,"alloc_pages,bitmap:%#018lx\n",*memory_management_struct.bits_map);

	tsk = (struct task_struct *)Phy_To_Virt(p->PHY_address);
	color_printk(WHITE,BLACK,"struct task_struct address:%#018lx\n",(unsigned long)tsk);

	memset(tsk,0,sizeof(*tsk));
	*tsk = *current;

	list_init(&tsk->list);

	tsk->priority = 2;
	tsk->pid++;
	tsk->preempt_count = 0;
	tsk->state = TASK_UNINTERRUPTIBLE;

	thd = (struct thread_struct *)(tsk + 1);
	memset(thd,0,sizeof(*thd));
	tsk->thread = thd;

	memcpy(regs,(void *)((unsigned long)tsk + STACK_SIZE - sizeof(struct pt_regs)),sizeof(struct pt_regs));

	thd->rsp0 = (unsigned long)tsk + STACK_SIZE;
	thd->rip = regs->rip;
	thd->rsp = (unsigned long)tsk + STACK_SIZE - sizeof(struct pt_regs);
	thd->fs = KERNEL_DS;
	thd->gs = KERNEL_DS;

	if(!(tsk->flags & PF_KTHREAD))
		thd->rip = regs->rip = (unsigned long)ret_system_call;

	tsk->state = TASK_RUNNING;
	insert_task_queue(tsk);

	return 1;
}



unsigned long do_exit(unsigned long code)
{
	color_printk(RED,BLACK,"exit task is running,arg:%#018lx\n",code);
	while(1);
}


unsigned long  system_call_function(struct pt_regs * regs)
{
	return system_call_table[regs->rax](regs);
}


extern void kernel_thread_func(void);
__asm__ (
"kernel_thread_func:	\n\t"
"	popq	%r15	\n\t"
"	popq	%r14	\n\t"	
"	popq	%r13	\n\t"	
"	popq	%r12	\n\t"	
"	popq	%r11	\n\t"	
"	popq	%r10	\n\t"	
"	popq	%r9	\n\t"	
"	popq	%r8	\n\t"	
"	popq	%rbx	\n\t"	
"	popq	%rcx	\n\t"	
"	popq	%rdx	\n\t"	
"	popq	%rsi	\n\t"	
"	popq	%rdi	\n\t"	
"	popq	%rbp	\n\t"	
"	popq	%rax	\n\t"	
"	movq	%rax,	%ds	\n\t"
"	popq	%rax		\n\t"
"	movq	%rax,	%es	\n\t"
"	popq	%rax		\n\t"
"	addq	$0x38,	%rsp	\n\t"
/////////////////////////////////
"	movq	%rdx,	%rdi	\n\t"
"	callq	*%rbx		\n\t"
"	movq	%rax,	%rdi	\n\t"
"	callq	do_exit		\n\t"
);



int kernel_thread(unsigned long (* fn)(unsigned long), unsigned long arg, unsigned long flags)
{
	struct pt_regs regs;
	memset(&regs,0,sizeof(regs));

	regs.rbx = (unsigned long)fn;
	regs.rdx = (unsigned long)arg;

	regs.ds = KERNEL_DS;
	regs.es = KERNEL_DS;
	regs.cs = KERNEL_CS;
	regs.ss = KERNEL_DS;
	regs.rflags = (1 << 9);
	regs.rip = (unsigned long)kernel_thread_func;

	return do_fork(&regs,flags,0,0);
}



inline void __switch_to(struct task_struct *prev,struct task_struct *next)
{

	init_tss[0].rsp0 = next->thread->rsp0;

	set_tss64(TSS64_Table,init_tss[0].rsp0, init_tss[0].rsp1, init_tss[0].rsp2, init_tss[0].ist1, init_tss[0].ist2, init_tss[0].ist3, init_tss[0].ist4, init_tss[0].ist5, init_tss[0].ist6, init_tss[0].ist7);

	__asm__ __volatile__("movq	%%fs,	%0 \n\t":"=a"(prev->thread->fs));
	__asm__ __volatile__("movq	%%gs,	%0 \n\t":"=a"(prev->thread->gs));

	__asm__ __volatile__("movq	%0,	%%fs \n\t"::"a"(next->thread->fs));
	__asm__ __volatile__("movq	%0,	%%gs \n\t"::"a"(next->thread->gs));

	wrmsr(0x175,next->thread->rsp0);

	color_printk(WHITE,BLACK,"prev->thread->rsp0:%#018lx\t",prev->thread->rsp0);
	color_printk(WHITE,BLACK,"prev->thread->rsp :%#018lx\n",prev->thread->rsp);
	color_printk(WHITE,BLACK,"next->thread->rsp0:%#018lx\t",next->thread->rsp0);
	color_printk(WHITE,BLACK,"next->thread->rsp :%#018lx\n",next->thread->rsp);
}

/*

*/

void task_init()
{
	init_mm.pgd = (pml4t_t *)Global_CR3;

	init_mm.start_code = memory_management_struct.start_code;
	init_mm.end_code = memory_management_struct.end_code;

	init_mm.start_data = (unsigned long)&_data;
	init_mm.end_data = memory_management_struct.end_data;

	init_mm.start_rodata = (unsigned long)&_rodata; 
	init_mm.end_rodata = (unsigned long)&_erodata;

	init_mm.start_brk = 0;
	init_mm.end_brk = memory_management_struct.end_brk;

	init_mm.start_stack = _stack_start;
	
	wrmsr(0x174,KERNEL_CS);
	wrmsr(0x175,current->thread->rsp0);
	wrmsr(0x176,(unsigned long)system_call);
	
//	init_thread,init_tss
	set_tss64(TSS64_Table,init_thread.rsp0, init_tss[0].rsp1, init_tss[0].rsp2, init_tss[0].ist1, init_tss[0].ist2, init_tss[0].ist3, init_tss[0].ist4, init_tss[0].ist5, init_tss[0].ist6, init_tss[0].ist7);

	init_tss[0].rsp0 = init_thread.rsp0;

	list_init(&init_task_union.task.list);

	kernel_thread(init,10,CLONE_FS | CLONE_FILES | CLONE_SIGNAL);

	init_task_union.task.preempt_count = 0;
	init_task_union.task.state = TASK_RUNNING;
}

