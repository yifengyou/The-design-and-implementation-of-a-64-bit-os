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

#include "trap.h"
#include "gate.h"
#include "ptrace.h"
#include "printk.h"
#include "SMP.h"

void display_regs(struct pt_regs * regs)
{
	color_printk(RED,BLACK,"CS:%#010x,SS:%#010x\nDS:%#010x,ES:%#010x\nRFLAGS:%#018lx\n",regs->cs,regs->ss,regs->ds,regs->es,regs->rflags);
	color_printk(RED,BLACK,"RAX:%#018lx,RBX:%#018lx,RCX:%#018lx,RDX:%#018lx\nRSP:%#018lx,RBP:%#018lx,RIP:%#018lx\nRSI:%#018lx,RDI:%#018lx\n",regs->rax,regs->rbx,regs->rcx,regs->rdx,regs->rsp,regs->rbp,regs->rip,regs->rsi,regs->rdi);
	color_printk(RED,BLACK,"R8 :%#018lx,R9 :%#018lx\nR10:%#018lx,R11:%#018lx\nR12:%#018lx,R13:%#018lx\nR14:%#018lx,R15:%#018lx\n",regs->r8,regs->r9,regs->r10,regs->r11,regs->r12,regs->r13,regs->r14,regs->r15);
}

/*
	0
*/

void do_divide_error(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_divide_error(0),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);
	display_regs(regs);
	while(1)
		hlt();
}

/*
	1
*/

void do_debug(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_debug(1),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);
	display_regs(regs);
	while(1)
		hlt();
}

/*
	2
*/

void do_nmi(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_nmi(2),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);
	display_regs(regs);
	while(1)
		hlt();
}

/*
	3
*/

void do_int3(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_int3(3),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);
	display_regs(regs);
	while(1)
		hlt();
}

/*
	4
*/

void do_overflow(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_overflow(4),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);
	display_regs(regs);
	while(1)
		hlt();
}

/*
	5
*/

void do_bounds(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_bounds(5),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code ,SMP_cpu_id(),current->pid);
	display_regs(regs);
	while(1)
		hlt();
}

/*
	6
*/

void do_undefined_opcode(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_undefined_opcode(6),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);
	display_regs(regs);
	while(1)
		hlt();
}

/*
	7
*/

void do_dev_not_available(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_dev_not_available(7),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);
	display_regs(regs);
	while(1)
		hlt();
}

/*
	8
*/

void do_double_fault(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_double_fault(8),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);
	display_regs(regs);
	while(1)
		hlt();
}

/*
	9
*/

void do_coprocessor_segment_overrun(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_coprocessor_segment_overrun(9),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);
	display_regs(regs);
	while(1)
		hlt();
}

/*
	10
*/

void do_invalid_TSS(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_invalid_TSS(10),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);

	if(error_code & 0x01)
		color_printk(RED,BLACK,"The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception.\n");

	if(error_code & 0x02)
		color_printk(RED,BLACK,"Refers to a gate descriptor in the IDT;\n");
	else
		color_printk(RED,BLACK,"Refers to a descriptor in the GDT or the current LDT;\n");

	if((error_code & 0x02) == 0)
		if(error_code & 0x04)
			color_printk(RED,BLACK,"Refers to a segment or gate descriptor in the LDT;\n");
		else
			color_printk(RED,BLACK,"Refers to a descriptor in the current GDT;\n");

	color_printk(RED,BLACK,"Segment Selector Index:%#010x\n",error_code & 0xfff8);

	display_regs(regs);
	while(1)
		hlt();
}

/*
	11
*/

void do_segment_not_present(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_segment_not_present(11),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);

	if(error_code & 0x01)
		color_printk(RED,BLACK,"The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception.\n");

	if(error_code & 0x02)
		color_printk(RED,BLACK,"Refers to a gate descriptor in the IDT;\n");
	else
		color_printk(RED,BLACK,"Refers to a descriptor in the GDT or the current LDT;\n");

	if((error_code & 0x02) == 0)
		if(error_code & 0x04)
			color_printk(RED,BLACK,"Refers to a segment or gate descriptor in the LDT;\n");
		else
			color_printk(RED,BLACK,"Refers to a descriptor in the current GDT;\n");

	color_printk(RED,BLACK,"Segment Selector Index:%#010x\n",error_code & 0xfff8);

	display_regs(regs);
	while(1)
		hlt();
}

/*
	12
*/

void do_stack_segment_fault(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_stack_segment_fault(12),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);

	if(error_code & 0x01)
		color_printk(RED,BLACK,"The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception.\n");

	if(error_code & 0x02)
		color_printk(RED,BLACK,"Refers to a gate descriptor in the IDT;\n");
	else
		color_printk(RED,BLACK,"Refers to a descriptor in the GDT or the current LDT;\n");

	if((error_code & 0x02) == 0)
		if(error_code & 0x04)
			color_printk(RED,BLACK,"Refers to a segment or gate descriptor in the LDT;\n");
		else
			color_printk(RED,BLACK,"Refers to a descriptor in the current GDT;\n");

	color_printk(RED,BLACK,"Segment Selector Index:%#010x\n",error_code & 0xfff8);

	display_regs(regs);
	while(1)
		hlt();
}

/*
	13
*/

void do_general_protection(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_general_protection(13),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);

	if(error_code & 0x01)
		color_printk(RED,BLACK,"The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception.\n");

	if(error_code & 0x02)
		color_printk(RED,BLACK,"Refers to a gate descriptor in the IDT;\n");
	else
		color_printk(RED,BLACK,"Refers to a descriptor in the GDT or the current LDT;\n");

	if((error_code & 0x02) == 0)
		if(error_code & 0x04)
			color_printk(RED,BLACK,"Refers to a segment or gate descriptor in the LDT;\n");
		else
			color_printk(RED,BLACK,"Refers to a descriptor in the current GDT;\n");

	color_printk(RED,BLACK,"Segment Selector Index:%#010x\n",error_code & 0xfff8);

	display_regs(regs);
	while(1)
		hlt();
}

/*
	14
*/

void do_page_fault(struct pt_regs * regs,unsigned long error_code)
{
	unsigned long cr2 = 0;

	__asm__	__volatile__("movq	%%cr2,	%0":"=r"(cr2)::"memory");

	color_printk(RED,BLACK,"do_page_fault(14),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);

	if(!(error_code & 0x01))
		color_printk(RED,BLACK,"Page Not-Present,\t");

	if(error_code & 0x02)
		color_printk(RED,BLACK,"Write Cause Fault,\t");
	else
		color_printk(RED,BLACK,"Read Cause Fault,\t");

	if(error_code & 0x04)
		color_printk(RED,BLACK,"Fault in user(3)\t");
	else
		color_printk(RED,BLACK,"Fault in supervisor(0,1,2)\t");

	if(error_code & 0x08)
		color_printk(RED,BLACK,",Reserved Bit Cause Fault\t");

	if(error_code & 0x10)
		color_printk(RED,BLACK,",Instruction fetch Cause Fault");

	color_printk(RED,BLACK,"\n");

	color_printk(RED,BLACK,"CR2:%#018lx\n",cr2);

	display_regs(regs);
	while(1)
		hlt();
}

/*
	16
*/

void do_x87_FPU_error(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_x87_FPU_error(16),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);
	display_regs(regs);
	while(1)
		hlt();
}

/*
	17
*/

void do_alignment_check(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_alignment_check(17),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);
	display_regs(regs);
	while(1)
		hlt();
}

/*
	18
*/

void do_machine_check(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_machine_check(18),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);
	display_regs(regs);
	while(1)
		hlt();
}

/*
	19
*/

void do_SIMD_exception(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_SIMD_exception(19),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);
	display_regs(regs);
	while(1)
		hlt();
}

/*
	20
*/

void do_virtualization_exception(struct pt_regs * regs,unsigned long error_code)
{
	color_printk(RED,BLACK,"do_virtualization_exception(20),ERROR_CODE:%#018lx,CPU:%#010x,PID:%#010x\n",error_code,SMP_cpu_id(),current->pid);
	display_regs(regs);
	while(1)
		hlt();
}

/*

*/

void sys_vector_init()
{
	set_trap_gate(0,0,divide_error);
	set_trap_gate(1,0,debug);
	set_intr_gate(2,0,nmi);
	set_system_gate(3,0,int3);
	set_system_gate(4,0,overflow);
	set_system_gate(5,0,bounds);
	set_trap_gate(6,0,undefined_opcode);
	set_trap_gate(7,0,dev_not_available);
	set_trap_gate(8,0,double_fault);
	set_trap_gate(9,0,coprocessor_segment_overrun);
	set_trap_gate(10,0,invalid_TSS);
	set_trap_gate(11,0,segment_not_present);
	set_trap_gate(12,0,stack_segment_fault);
	set_trap_gate(13,0,general_protection);
	set_trap_gate(14,0,page_fault);
	//15 Intel reserved. Do not use.
	set_trap_gate(16,0,x87_FPU_error);
	set_trap_gate(17,0,alignment_check);
	set_trap_gate(18,0,machine_check);
	set_trap_gate(19,0,SIMD_exception);
	set_trap_gate(20,0,virtualization_exception);

	//set_system_gate(SYSTEM_CALL_VECTOR,7,system_call);

}

