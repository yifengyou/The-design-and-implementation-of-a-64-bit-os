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

#include "interrupt.h"
#include "linkage.h"
#include "lib.h"
#include "printk.h"
#include "memory.h"
#include "gate.h"
#include "ptrace.h"
#include "cpu.h"
#include "APIC.h"

/*

*/

void Local_APIC_init()
{
	unsigned int x,y;
	unsigned int a,b,c,d;

	//check APIC & x2APIC support
	get_cpuid(1,0,&a,&b,&c,&d);
	//void get_cpuid(unsigned int Mop,unsigned int Sop,unsigned int * a,unsigned int * b,unsigned int * c,unsigned int * d)
	color_printk(WHITE,BLACK,"CPUID\t01,eax:%#010x,ebx:%#010x,ecx:%#010x,edx:%#010x\n",a,b,c,d);

	if((1<<9) & d)
		color_printk(WHITE,BLACK,"HW support APIC&xAPIC\t");
	else
		color_printk(WHITE,BLACK,"HW NO support APIC&xAPIC\t");
	
	if((1<<21) & c)
		color_printk(WHITE,BLACK,"HW support x2APIC\n");
	else
		color_printk(WHITE,BLACK,"HW NO support x2APIC\n");

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

	color_printk(WHITE,BLACK,"eax:%#010x,edx:%#010x\t",x,y);
	
	if(x&0xc00)
		color_printk(WHITE,BLACK,"xAPIC & x2APIC enabled\n");

	//enable SVR[8]
	__asm__ __volatile__(	"movq 	$0x80f,	%%rcx	\n\t"
				"rdmsr	\n\t"
				"bts	$8,	%%rax	\n\t"
				"bts	$12,%%rax\n\t"
				"wrmsr	\n\t"
				"movq 	$0x80f,	%%rcx	\n\t"
				"rdmsr	\n\t"
				:"=a"(x),"=d"(y)
				:
				:"memory");

	color_printk(WHITE,BLACK,"eax:%#010x,edx:%#010x\t",x,y);

	if(x&0x100)
		color_printk(WHITE,BLACK,"SVR[8] enabled\n");
	if(x&0x1000)
		color_printk(WHITE,BLACK,"SVR[12] enabled\n");

	//get local APIC ID
	__asm__ __volatile__(	"movq $0x802,	%%rcx	\n\t"
				"rdmsr	\n\t"
				:"=a"(x),"=d"(y)
				:
				:"memory");
	
	color_printk(WHITE,BLACK,"eax:%#010x,edx:%#010x\tx2APIC ID:%#010x\n",x,y,x);

	//get local APIC version
	__asm__ __volatile__(	"movq $0x803,	%%rcx	\n\t"
				"rdmsr	\n\t"
				:"=a"(x),"=d"(y)
				:
				:"memory");

	color_printk(WHITE,BLACK,"local APIC Version:%#010x,Max LVT Entry:%#010x,SVR(Suppress EOI Broadcast):%#04x\t",x & 0xff,(x >> 16 & 0xff) + 1,x >> 24 & 0x1);

	if((x & 0xff) < 0x10)
		color_printk(WHITE,BLACK,"82489DX discrete APIC\n");

	else if( ((x & 0xff) >= 0x10) && ((x & 0xff) <= 0x15) )
		color_printk(WHITE,BLACK,"Integrated APIC\n");

	//mask all LVT	
	__asm__ __volatile__(	"movq 	$0x82f,	%%rcx	\n\t"	//CMCI
				"wrmsr	\n\t"
				"movq 	$0x832,	%%rcx	\n\t"	//Timer
				"wrmsr	\n\t"
				"movq 	$0x833,	%%rcx	\n\t"	//Thermal Monitor
				"wrmsr	\n\t"
				"movq 	$0x834,	%%rcx	\n\t"	//Performance Counter
				"wrmsr	\n\t"
				"movq 	$0x835,	%%rcx	\n\t"	//LINT0
				"wrmsr	\n\t"
				"movq 	$0x836,	%%rcx	\n\t"	//LINT1
				"wrmsr	\n\t"
				"movq 	$0x837,	%%rcx	\n\t"	//Error
				"wrmsr	\n\t"
				:
				:"a"(0x10000),"d"(0x00)
				:"memory");

	color_printk(GREEN,BLACK,"Mask ALL LVT\n");

	//TPR
	__asm__ __volatile__(	"movq 	$0x808,	%%rcx	\n\t"
				"rdmsr	\n\t"
				:"=a"(x),"=d"(y)
				:
				:"memory");

	color_printk(GREEN,BLACK,"Set LVT TPR:%#010x\t",x);

	//PPR
	__asm__ __volatile__(	"movq 	$0x80a,	%%rcx	\n\t"
				"rdmsr	\n\t"
				:"=a"(x),"=d"(y)
				:
				:"memory");

	color_printk(GREEN,BLACK,"Set LVT PPR:%#010x\n",x);

}

/*

*/

void APIC_IOAPIC_init()
{
	//	init trap abort fault
	int i ;

	for(i = 32;i < 56;i++)
	{
		set_intr_gate(i , 2 , interrupt[i - 32]);
	}

	//mask 8259A
	color_printk(GREEN,BLACK,"MASK 8259A\n");
	io_out8(0x21,0xff);
	io_out8(0xa1,0xff);

	//init local apic
	Local_APIC_init();

	//enable IF eflages
	sti();
}

/*

*/

void do_IRQ(struct pt_regs * regs,unsigned long nr)	//regs:rsp,nr
{

}

