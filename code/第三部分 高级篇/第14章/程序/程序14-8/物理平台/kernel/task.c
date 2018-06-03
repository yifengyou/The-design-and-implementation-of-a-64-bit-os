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
#include "SMP.h"
#include "unistd.h"
#include "stdio.h"
#include "sched.h"
#include "errno.h"
#include "fcntl.h"

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

long global_pid;

struct task_struct *get_task(long pid)
{
	struct task_struct *tsk = NULL;
	for(tsk = init_task_union.task.next;tsk != &init_task_union.task;tsk = tsk->next)
	{
		if(tsk->pid == pid)
			return tsk;
	}
	return NULL;
}


struct file * open_exec_file(char * path)
{
	struct dir_entry * dentry = NULL;
	struct file * filp = NULL;

	dentry = path_walk(path,0);

	if(dentry == NULL)
		return (void *)-ENOENT;
	if(dentry->dir_inode->attribute == FS_ATTR_DIR)
		return (void *)-ENOTDIR;

	filp = (struct file *)kmalloc(sizeof(struct file),0);
	if(filp == NULL)
		return (void *)-ENOMEM;

	filp->position = 0;
	filp->mode = 0;
	filp->dentry = dentry;
	filp->mode = O_RDONLY;
	filp->f_ops = dentry->dir_inode->f_ops;

	return filp;
}


unsigned long do_execve(struct pt_regs *regs,char *name)
{
	unsigned long code_start_addr = 0x800000;
	unsigned long stack_start_addr = 0xa00000;
	unsigned long brk_start_addr = 0xc00000;
	unsigned long * tmp;
	unsigned long * virtual = NULL;
	struct Page * p = NULL;
	struct file * filp = NULL;
	unsigned long retval = 0;
	long pos = 0;

	regs->ds = USER_DS;
	regs->es = USER_DS;
	regs->ss = USER_DS;
	regs->cs = USER_CS;
//	regs->rip = new_rip;
//	regs->rsp = new_rsp;
	regs->r10 = 0x800000;
	regs->r11 = 0xa00000;
	regs->rax = 1;

	color_printk(RED,BLACK,"do_execve task is running\n");

	if(current->flags & PF_VFORK)
	{
		current->mm = (struct mm_struct *)kmalloc(sizeof(struct mm_struct),0);
		memset(current->mm,0,sizeof(struct mm_struct));

		current->mm->pgd = (pml4t_t *)Virt_To_Phy(kmalloc(PAGE_4K_SIZE,0));
		color_printk(RED,BLACK,"load_binary_file malloc new pgd:%#018lx\n",current->mm->pgd);
		memset(Phy_To_Virt(current->mm->pgd),0,PAGE_4K_SIZE/2);
		memcpy(Phy_To_Virt(init_task[SMP_cpu_id()]->mm->pgd)+256,Phy_To_Virt(current->mm->pgd)+256,PAGE_4K_SIZE/2);	//copy kernel space
	}

	tmp = Phy_To_Virt((unsigned long *)((unsigned long)current->mm->pgd & (~ 0xfffUL)) + ((code_start_addr >> PAGE_GDT_SHIFT) & 0x1ff));
	if(*tmp == NULL)
	{
		virtual = kmalloc(PAGE_4K_SIZE,0);
		memset(virtual,0,PAGE_4K_SIZE);
		set_mpl4t(tmp,mk_mpl4t(Virt_To_Phy(virtual),PAGE_USER_GDT));
	}

	tmp = Phy_To_Virt((unsigned long *)(*tmp & (~ 0xfffUL)) + ((code_start_addr >> PAGE_1G_SHIFT) & 0x1ff));
	if(*tmp == NULL)
	{
		virtual = kmalloc(PAGE_4K_SIZE,0);
		memset(virtual,0,PAGE_4K_SIZE);
		set_pdpt(tmp,mk_pdpt(Virt_To_Phy(virtual),PAGE_USER_Dir));
	}

	tmp = Phy_To_Virt((unsigned long *)(*tmp & (~ 0xfffUL)) + ((code_start_addr >> PAGE_2M_SHIFT) & 0x1ff));
	if(*tmp == NULL)
	{
		p = alloc_pages(ZONE_NORMAL,1,PG_PTable_Maped);
		set_pdt(tmp,mk_pdt(p->PHY_address,PAGE_USER_Page));
	}
	__asm__ __volatile__ ("movq	%0,	%%cr3	\n\t"::"r"(current->mm->pgd):"memory");


	if(!(current->flags & PF_KTHREAD))
		current->addr_limit = TASK_SIZE;

	current->mm->start_code = code_start_addr;
	current->mm->end_code = 0;
	current->mm->start_data = 0;
	current->mm->end_data = 0;
	current->mm->start_rodata = 0;
	current->mm->end_rodata = 0;
	current->mm->start_bss = 0;
	current->mm->end_bss = 0;
	current->mm->start_brk = brk_start_addr;
	current->mm->end_brk = brk_start_addr;
	current->mm->start_stack = stack_start_addr;
	
	exit_files(current);

	current->flags &= ~PF_VFORK;

	filp = open_exec_file(name);

	if((unsigned long)filp > -0x1000UL)
		return (unsigned long)filp;

	memset((void *)0x800000,0,PAGE_2M_SIZE);
	retval = filp->f_ops->read(filp,(void *)0x800000,filp->dentry->dir_inode->file_size,&pos);

	return retval;
}


unsigned long init(unsigned long arg)
{
	DISK1_FAT32_FS_init();

	color_printk(RED,BLACK,"init task is running,arg:%#018lx\n",arg);

	current->thread->rip = (unsigned long)ret_system_call;
	current->thread->rsp = (unsigned long)current + STACK_SIZE - sizeof(struct pt_regs);
	current->thread->gs = USER_DS;
	current->thread->fs = USER_DS;
	current->flags &= ~PF_KTHREAD;

	__asm__	__volatile__	(	"movq	%1,	%%rsp	\n\t"
					"pushq	%2		\n\t"
					"jmp	do_execve	\n\t"
					:
					:"D"(current->thread->rsp),"m"(current->thread->rsp),"m"(current->thread->rip),"S"("/init.bin")
					:"memory"
				);

	return 1;
}


void kernel_thread_func(void);
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


inline void wakeup_process(struct task_struct *tsk)
{
	tsk->state = TASK_RUNNING;
	insert_task_queue(tsk);
}

unsigned long copy_flags(unsigned long clone_flags,struct task_struct *tsk)
{
	if(clone_flags & CLONE_VM)
		tsk->flags |= PF_VFORK;
	return 0;
}

unsigned long copy_files(unsigned long clone_flags,struct task_struct *tsk)
{
	int error = 0;
	int i = 0;
	if(clone_flags & CLONE_FS)
		goto out;
	
	for(i = 0;i<TASK_FILE_MAX;i++)
		if(current->file_struct[i] != NULL)
		{
			tsk->file_struct[i] = (struct file *)kmalloc(sizeof(struct file),0);
			memcpy(current->file_struct[i],tsk->file_struct[i],sizeof(struct file));
		}
out:
	return error;
}
void exit_files(struct task_struct *tsk)
{
	int i = 0;
	if(tsk->flags & PF_VFORK)
		;
	else
		for(i = 0;i<TASK_FILE_MAX;i++)
			if(tsk->file_struct[i] != NULL)
			{
				kfree(tsk->file_struct[i]);
			}

	memset(tsk->file_struct,0,sizeof(struct file *) * TASK_FILE_MAX);	//clear current->file_struct
}


unsigned long copy_mm(unsigned long clone_flags,struct task_struct *tsk)
{
	int error = 0;
	struct mm_struct *newmm = NULL;
	unsigned long code_start_addr = 0x800000;
	unsigned long stack_start_addr = 0xa00000;
	unsigned long brk_start_addr = 0xc00000;
	unsigned long * tmp;
	unsigned long * virtual = NULL;
	struct Page * p = NULL;

	if(clone_flags & CLONE_VM)
	{
		newmm = current->mm;
		goto out;
	}

	newmm = (struct mm_struct *)kmalloc(sizeof(struct mm_struct),0);
	memcpy(current->mm,newmm,sizeof(struct mm_struct));

	newmm->pgd = (pml4t_t *)Virt_To_Phy(kmalloc(PAGE_4K_SIZE,0));
	memcpy(Phy_To_Virt(init_task[SMP_cpu_id()]->mm->pgd)+256,Phy_To_Virt(newmm->pgd)+256,PAGE_4K_SIZE/2);	//copy kernel space

	memset(Phy_To_Virt(newmm->pgd),0,PAGE_4K_SIZE/2);	//copy user code & data & bss space

	tmp = Phy_To_Virt((unsigned long *)((unsigned long)newmm->pgd & (~ 0xfffUL)) + ((code_start_addr >> PAGE_GDT_SHIFT) & 0x1ff));
	virtual = kmalloc(PAGE_4K_SIZE,0);
	memset(virtual,0,PAGE_4K_SIZE);
	set_mpl4t(tmp,mk_mpl4t(Virt_To_Phy(virtual),PAGE_USER_GDT));

	tmp = Phy_To_Virt((unsigned long *)(*tmp & (~ 0xfffUL)) + ((code_start_addr >> PAGE_1G_SHIFT) & 0x1ff));
	virtual = kmalloc(PAGE_4K_SIZE,0);
	memset(virtual,0,PAGE_4K_SIZE);
	set_pdpt(tmp,mk_pdpt(Virt_To_Phy(virtual),PAGE_USER_Dir));

	tmp = Phy_To_Virt((unsigned long *)(*tmp & (~ 0xfffUL)) + ((code_start_addr >> PAGE_2M_SHIFT) & 0x1ff));
	p = alloc_pages(ZONE_NORMAL,1,PG_PTable_Maped);
	set_pdt(tmp,mk_pdt(p->PHY_address,PAGE_USER_Page));	

	memcpy((void *)code_start_addr,Phy_To_Virt(p->PHY_address),stack_start_addr - code_start_addr);

	////copy user brk space
	if(current->mm->end_brk - current->mm->start_brk != 0)
	{
		tmp = Phy_To_Virt((unsigned long *)((unsigned long)newmm->pgd & (~ 0xfffUL)) + ((brk_start_addr >> PAGE_GDT_SHIFT) & 0x1ff));
		tmp = Phy_To_Virt((unsigned long *)(*tmp & (~ 0xfffUL)) + ((brk_start_addr >> PAGE_1G_SHIFT) & 0x1ff));
		tmp = Phy_To_Virt((unsigned long *)(*tmp & (~ 0xfffUL)) + ((brk_start_addr >> PAGE_2M_SHIFT) & 0x1ff));
		p = alloc_pages(ZONE_NORMAL,1,PG_PTable_Maped);
		set_pdt(tmp,mk_pdt(p->PHY_address,PAGE_USER_Page));

		memcpy((void *)brk_start_addr,Phy_To_Virt(p->PHY_address),PAGE_2M_SIZE);
	}

out:
	tsk->mm = newmm;
	return error;
}
void exit_mm(struct task_struct *tsk)
{
	unsigned long code_start_addr = 0x800000;
	unsigned long * tmp4;
	unsigned long * tmp3;
	unsigned long * tmp2;
	
	if(tsk->flags & PF_VFORK)
		return;

	if(tsk->mm->pgd != NULL)
	{
		tmp4 = Phy_To_Virt((unsigned long *)((unsigned long)tsk->mm->pgd & (~ 0xfffUL)) + ((code_start_addr >> PAGE_GDT_SHIFT) & 0x1ff));
		tmp3 = Phy_To_Virt((unsigned long *)(*tmp4 & (~ 0xfffUL)) + ((code_start_addr >> PAGE_1G_SHIFT) & 0x1ff));
		tmp2 = Phy_To_Virt((unsigned long *)(*tmp3 & (~ 0xfffUL)) + ((code_start_addr >> PAGE_2M_SHIFT) & 0x1ff));
		
		free_pages(Phy_to_2M_Page(*tmp2),1);
		kfree(Phy_To_Virt(*tmp3));
		kfree(Phy_To_Virt(*tmp4));
		kfree(Phy_To_Virt(tsk->mm->pgd));
	}
	if(tsk->mm != NULL)
		kfree(tsk->mm);
}


unsigned long copy_thread(unsigned long clone_flags,unsigned long stack_start,unsigned long stack_size,struct task_struct *tsk,struct pt_regs * regs)
{
	struct thread_struct *thd = NULL;
	struct pt_regs *childregs = NULL;

	thd = (struct thread_struct *)(tsk + 1);
	memset(thd,0,sizeof(*thd));
	tsk->thread = thd;

	childregs = (struct pt_regs *)((unsigned long)tsk + STACK_SIZE) - 1;

	memcpy(regs,childregs,sizeof(struct pt_regs));
	childregs->rax = 0;
	childregs->rsp = stack_start;

	thd->rsp0 = (unsigned long)tsk + STACK_SIZE;
	thd->rsp = (unsigned long)childregs;
	thd->fs = current->thread->fs;
	thd->gs = current->thread->gs;

	if(tsk->flags & PF_KTHREAD)
		thd->rip = (unsigned long)kernel_thread_func;
	else
		thd->rip = (unsigned long)ret_system_call; 

	color_printk(WHITE,BLACK,"current user ret addr:%#018lx,rsp:%#018lx\n",regs->r10,regs->r11);
	color_printk(WHITE,BLACK,"new user ret addr:%#018lx,rsp:%#018lx\n",childregs->r10,childregs->r11);

	return 0;
}
void exit_thread(struct task_struct *tsk){}


unsigned long do_fork(struct pt_regs * regs, unsigned long clone_flags, unsigned long stack_start, unsigned long stack_size)
{
	int retval = 0;
	struct task_struct *tsk = NULL;

//	alloc & copy task struct
	tsk = (struct task_struct *)kmalloc(STACK_SIZE,0);
	color_printk(WHITE,BLACK,"struct task_struct address:%#018lx\n",(unsigned long)tsk);

	if(tsk == NULL)
	{
		retval = -EAGAIN;
		goto alloc_copy_task_fail;
	}

	memset(tsk,0,sizeof(*tsk));
	memcpy(current,tsk,sizeof(struct task_struct));

	list_init(&tsk->list);
	tsk->priority = 2;
	tsk->pid = global_pid++;
	tsk->preempt_count = 0;
	tsk->cpu_id = SMP_cpu_id();
	tsk->state = TASK_UNINTERRUPTIBLE;
	tsk->next = init_task_union.task.next;
	init_task_union.task.next = tsk;
	tsk->parent = current;

	retval = -ENOMEM;
//	copy flags
	if(copy_flags(clone_flags,tsk))
		goto copy_flags_fail;

//	copy mm struct
	if(copy_mm(clone_flags,tsk))
		goto copy_mm_fail;

//	copy file struct
	if(copy_files(clone_flags,tsk))
		goto copy_files_fail;

//	copy thread struct
	if(copy_thread(clone_flags,stack_start,stack_size,tsk,regs))
		goto copy_thread_fail;

	retval = tsk->pid;
	wakeup_process(tsk);

fork_ok:
	return retval;

copy_thread_fail:
	exit_thread(tsk);
copy_files_fail:
	exit_files(tsk);
copy_mm_fail:
	exit_mm(tsk);
copy_flags_fail:
alloc_copy_task_fail:
	kfree(tsk);

	return retval;
}


unsigned long do_exit(unsigned long exit_code)
{
	color_printk(RED,BLACK,"exit task is running,arg:%#018lx\n",exit_code);
	while(1);
}


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

	return do_fork(&regs,flags | CLONE_VM,0,0);
}

inline void switch_mm(struct task_struct *prev,struct task_struct *next)
{
	__asm__ __volatile__ ("movq	%0,	%%cr3	\n\t"::"r"(next->mm->pgd):"memory");
}

inline void __switch_to(struct task_struct *prev,struct task_struct *next)
{
	init_tss[SMP_cpu_id()].rsp0 = next->thread->rsp0;

	__asm__ __volatile__("movq	%%fs,	%0 \n\t":"=a"(prev->thread->fs));
	__asm__ __volatile__("movq	%%gs,	%0 \n\t":"=a"(prev->thread->gs));

	__asm__ __volatile__("movq	%0,	%%fs \n\t"::"a"(next->thread->fs));
	__asm__ __volatile__("movq	%0,	%%gs \n\t"::"a"(next->thread->gs));

	wrmsr(0x175,next->thread->rsp0);
}

/*

*/

void task_init()
{
	unsigned long * tmp = NULL;
	unsigned long * vaddr = NULL;
	int i = 0;

	vaddr = (unsigned long *)Phy_To_Virt((unsigned long)Get_gdt() & (~ 0xfffUL));

	*vaddr = 0UL;
	
	for(i = 256;i<512;i++)
	{
		tmp = vaddr + i;

		if(*tmp == 0)
		{			
			unsigned long * virtual = kmalloc(PAGE_4K_SIZE,0);
			memset(virtual,0,PAGE_4K_SIZE);
			set_mpl4t(tmp,mk_mpl4t(Virt_To_Phy(virtual),PAGE_KERNEL_GDT));
		}
	}

	init_mm.pgd = (pml4t_t *)Get_gdt();

	init_mm.start_code = memory_management_struct.start_code;
	init_mm.end_code = memory_management_struct.end_code;

	init_mm.start_data = (unsigned long)&_data;
	init_mm.end_data = memory_management_struct.end_data;

	init_mm.start_rodata = (unsigned long)&_rodata; 
	init_mm.end_rodata = memory_management_struct.end_rodata;

	init_mm.start_bss = (unsigned long)&_bss;
	init_mm.end_bss = (unsigned long)&_ebss;

	init_mm.start_brk = memory_management_struct.start_brk;
	init_mm.end_brk = current->addr_limit;

	init_mm.start_stack = _stack_start;
	
	wrmsr(0x174,KERNEL_CS);
	wrmsr(0x175,current->thread->rsp0);
	wrmsr(0x176,(unsigned long)system_call);
	
//	init_thread,init_tss
//	set_tss64(TSS64_Table,init_thread.rsp0, init_tss[0].rsp1, init_tss[0].rsp2, init_tss[0].ist1, init_tss[0].ist2, init_tss[0].ist3, init_tss[0].ist4, init_tss[0].ist5, init_tss[0].ist6, init_tss[0].ist7);

	init_tss[SMP_cpu_id()].rsp0 = init_thread.rsp0;

	list_init(&init_task_union.task.list);

	kernel_thread(init,10,CLONE_FS | CLONE_SIGNAL);

	init_task_union.task.preempt_count = 0;
	init_task_union.task.state = TASK_RUNNING;
	init_task_union.task.cpu_id = 0;
}

