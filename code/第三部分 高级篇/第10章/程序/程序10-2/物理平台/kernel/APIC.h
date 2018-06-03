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

#ifndef __APIC_H__
#define __APIC_H__

#include "linkage.h"
#include "ptrace.h"
#include "interrupt.h"

/* 

*/

struct IOAPIC_map
{
	unsigned int physical_address;
	unsigned char * virtual_index_address;
	unsigned int *  virtual_data_address;
	unsigned int *  virtual_EOI_address;
}ioapic_map;

unsigned long ioapic_rte_read(unsigned char index);
void ioapic_rte_write(unsigned char index,unsigned long value);

/* 

*/

void IOAPIC_pagetable_remap();

/*

*/

void do_IRQ(struct pt_regs * regs,unsigned long nr);

/*

*/

void APIC_IOAPIC_init();
void Local_APIC_init();
void IOAPIC_init();

#endif
