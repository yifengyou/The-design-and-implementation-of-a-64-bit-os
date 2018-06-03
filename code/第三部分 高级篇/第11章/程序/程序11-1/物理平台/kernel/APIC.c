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

void IOAPIC_enable(unsigned long irq)
{
	unsigned long value = 0;
	value = ioapic_rte_read((irq - 32) * 2 + 0x10);
	value = value & (~0x10000UL); 
	ioapic_rte_write((irq - 32) * 2 + 0x10,value);
}

void IOAPIC_disable(unsigned long irq)
{
	unsigned long value = 0;
	value = ioapic_rte_read((irq - 32) * 2 + 0x10);
	value = value | 0x10000UL; 
	ioapic_rte_write((irq - 32) * 2 + 0x10,value);
}

unsigned long IOAPIC_install(unsigned long irq,void * arg)
{
	struct IO_APIC_RET_entry *entry = (struct IO_APIC_RET_entry *)arg;
	ioapic_rte_write((irq - 32) * 2 + 0x10,*(unsigned long *)entry);

	return 1;
}

void IOAPIC_uninstall(unsigned long irq)
{
	ioapic_rte_write((irq - 32) * 2 + 0x10,0x10000UL);
}

void IOAPIC_level_ack(unsigned long irq)
{
	__asm__ __volatile__(	"movq	$0x00,	%%rdx	\n\t"
				"movq	$0x00,	%%rax	\n\t"
				"movq 	$0x80b,	%%rcx	\n\t"
				"wrmsr	\n\t"
				:::"memory");
				
	*ioapic_map.virtual_EOI_address = 0;
}

void IOAPIC_edge_ack(unsigned long irq)
{
	__asm__ __volatile__(	"movq	$0x00,	%%rdx	\n\t"
				"movq	$0x00,	%%rax	\n\t"
				"movq 	$0x80b,	%%rcx	\n\t"
				"wrmsr	\n\t"
				:::"memory");
}

/*

*/

unsigned long ioapic_rte_read(unsigned char index)
{
	unsigned long ret;

	*ioapic_map.virtual_index_address = index + 1;
	io_mfence();
	ret = *ioapic_map.virtual_data_address;
	ret <<= 32;
	io_mfence();

	*ioapic_map.virtual_index_address = index;		
	io_mfence();
	ret |= *ioapic_map.virtual_data_address;
	io_mfence();

	return ret;
}

/*

*/

void ioapic_rte_write(unsigned char index,unsigned long value)
{
	*ioapic_map.virtual_index_address = index;
	io_mfence();
	*ioapic_map.virtual_data_address = value & 0xffffffff;
	value >>= 32;
	io_mfence();
	
	*ioapic_map.virtual_index_address = index + 1;
	io_mfence();
	*ioapic_map.virtual_data_address = value & 0xffffffff;
	io_mfence();
}

/*

*/

void IOAPIC_pagetable_remap()
{
	unsigned long * tmp;
	unsigned char * IOAPIC_addr = (unsigned char *)Phy_To_Virt(0xfec00000);

	ioapic_map.physical_address = 0xfec00000;
	ioapic_map.virtual_index_address  = IOAPIC_addr;
	ioapic_map.virtual_data_address   = (unsigned int *)(IOAPIC_addr + 0x10);
	ioapic_map.virtual_EOI_address    = (unsigned int *)(IOAPIC_addr + 0x40);
	
	Global_CR3 = Get_gdt();

	tmp = Phy_To_Virt(Global_CR3 + (((unsigned long)IOAPIC_addr >> PAGE_GDT_SHIFT) & 0x1ff));
	if (*tmp == 0)
	{
		unsigned long * virtual = kmalloc(PAGE_4K_SIZE,0);
		set_mpl4t(tmp,mk_mpl4t(Virt_To_Phy(virtual),PAGE_KERNEL_GDT));
	}

	color_printk(YELLOW,BLACK,"1:%#018lx\t%#018lx\n",(unsigned long)tmp,(unsigned long)*tmp);

	tmp = Phy_To_Virt((unsigned long *)(*tmp & (~ 0xfffUL)) + (((unsigned long)IOAPIC_addr >> PAGE_1G_SHIFT) & 0x1ff));
	if(*tmp == 0)
	{
		unsigned long * virtual = kmalloc(PAGE_4K_SIZE,0);
		set_pdpt(tmp,mk_pdpt(Virt_To_Phy(virtual),PAGE_KERNEL_Dir));
	}

	color_printk(YELLOW,BLACK,"2:%#018lx\t%#018lx\n",(unsigned long)tmp,(unsigned long)*tmp);
	
	tmp = Phy_To_Virt((unsigned long *)(*tmp & (~ 0xfffUL)) + (((unsigned long)IOAPIC_addr >> PAGE_2M_SHIFT) & 0x1ff));
	set_pdt(tmp,mk_pdt(ioapic_map.physical_address,PAGE_KERNEL_Page | PAGE_PWT | PAGE_PCD));

	color_printk(BLUE,BLACK,"3:%#018lx\t%#018lx\n",(unsigned long)tmp,(unsigned long)*tmp);

	color_printk(BLUE,BLACK,"ioapic_map.physical_address:%#010x\t\t\n",ioapic_map.physical_address);
	color_printk(BLUE,BLACK,"ioapic_map.virtual_address:%#018lx\t\t\n",(unsigned long)ioapic_map.virtual_index_address);

	flush_tlb();
}

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
				"bts	$12,	%%rax\n\t"
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

void IOAPIC_init()
{
	int i ;
	//	I/O APIC
	//	I/O APIC	ID	
	*ioapic_map.virtual_index_address = 0x00;
	io_mfence();
	*ioapic_map.virtual_data_address = 0x0f000000;
	io_mfence();
	color_printk(GREEN,BLACK,"Get IOAPIC ID REG:%#010x,ID:%#010x\n",*ioapic_map.virtual_data_address, *ioapic_map.virtual_data_address >> 24 & 0xf);
	io_mfence();

	//	I/O APIC	Version
	*ioapic_map.virtual_index_address = 0x01;
	io_mfence();
	color_printk(GREEN,BLACK,"Get IOAPIC Version REG:%#010x,MAX redirection enties:%#08d\n",*ioapic_map.virtual_data_address ,((*ioapic_map.virtual_data_address >> 16) & 0xff) + 1);

	//RTE	
	for(i = 0x10;i < 0x40;i += 2)
		ioapic_rte_write(i,0x10020 + ((i - 0x10) >> 1));

	color_printk(GREEN,BLACK,"I/O APIC Redirection Table Entries Set Finished.\n");	
}

/*

*/

void APIC_IOAPIC_init()
{
	//	init trap abort fault
	int i ;
	unsigned int x;
	unsigned int * p;

	IOAPIC_pagetable_remap();

	for(i = 32;i < 56;i++)
	{
		set_intr_gate(i , 2 , interrupt[i - 32]);
	}

	//mask 8259A
	color_printk(GREEN,BLACK,"MASK 8259A\n");
	io_out8(0x21,0xff);
	io_out8(0xa1,0xff);

	//enable IMCR
	io_out8(0x22,0x70);
	io_out8(0x23,0x01);	
	
	//init local apic
	Local_APIC_init();

	//init ioapic
	IOAPIC_init();

	//get RCBA address
	io_out32(0xcf8,0x8000f8f0);
	x = io_in32(0xcfc);
	color_printk(RED,BLACK,"Get RCBA Address:%#010x\n",x);	
	x = x & 0xffffc000;
	color_printk(RED,BLACK,"Get RCBA Address:%#010x\n",x);	

	//get OIC address
	if(x > 0xfec00000 && x < 0xfee00000)
	{
		p = (unsigned int *)Phy_To_Virt(x + 0x31feUL);
	}

	//enable IOAPIC
	x = (*p & 0xffffff00) | 0x100;
	io_mfence();
	*p = x;
	io_mfence();

	memset(interrupt_desc,0,sizeof(irq_desc_T)*NR_IRQS);
	
	//open IF eflages
	sti();
}

/*

*/

void do_IRQ(struct pt_regs * regs,unsigned long nr)	//regs:rsp,nr
{
	irq_desc_T * irq = &interrupt_desc[nr - 32];

	if(irq->handler != NULL)
		irq->handler(nr,irq->parameter,regs);

	if(irq->controller != NULL && irq->controller->ack != NULL)
		irq->controller->ack(nr);
}

