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

#include "backtrace.h"
#include "printk.h"

extern unsigned long kallsyms_addresses[] __attribute__((weak));;
extern long kallsyms_syms_num __attribute__((weak));;
extern long kallsyms_index[] __attribute__((weak));;
extern char* kallsyms_names __attribute__((weak));;

int lookup_kallsyms(unsigned long address)
{
	int index = 0;
	char * string =(char *) &kallsyms_names;
	for(index = 0;index<kallsyms_syms_num;index++)
		if(address > kallsyms_addresses[index] && address <= kallsyms_addresses[index+1])
			break;
	if(index < kallsyms_syms_num)
	{
		color_printk(WHITE,BLUE,"backtrace address:%#018lx (+) %04d\tbacktrace function:%s(%#018lx)\n",address,address - kallsyms_addresses[index],&string[kallsyms_index[index]],kallsyms_addresses[index]);
		return 0;
	}
	else
		return 1;
}

void backtrace(struct pt_regs * regs)
{
	unsigned long *rbp = (unsigned long *)regs->rbp;
	unsigned long ret_address = *(rbp+1);
	int i = 0;

	lookup_kallsyms(regs->rip);
	for(i = 0;i<10;i++)
	{
		if(lookup_kallsyms(ret_address))
			break;
		rbp = (unsigned long *)*rbp;
		ret_address = *(rbp+1);
	}
}



