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

#include "memory.h"
#include "lib.h"

void init_memory()
{
	int i,j;
	unsigned long TotalMem = 0 ;
	struct E820 *p = NULL;	
	
	color_printk(BLUE,BLACK,"Display Physics Address MAP,Type(1:RAM,2:ROM or Reserved,3:ACPI Reclaim Memory,4:ACPI NVS Memory,Others:Undefine)\n");
	p = (struct E820 *)0xffff800000007e00;

	for(i = 0;i < 32;i++)
	{
		color_printk(ORANGE,BLACK,"Address:%#018lx\tLength:%#018lx\tType:%#010x\n",p->address,p->length,p->type);
		unsigned long tmp = 0;
		if(p->type == 1)
			TotalMem +=  p->length;

		memory_management_struct.e820[i].address += p->address;

		memory_management_struct.e820[i].length	 += p->length;

		memory_management_struct.e820[i].type	 = p->type;
		
		memory_management_struct.e820_length = i;

		p++;
		if(p->type > 4)
			break;		
	}

	color_printk(ORANGE,BLACK,"OS Can Used Total RAM:%#018lx\n",TotalMem);

	TotalMem = 0;

	for(i = 0;i <= memory_management_struct.e820_length;i++)
	{
		unsigned long start,end;
		if(memory_management_struct.e820[i].type != 1)
			continue;
		start = PAGE_2M_ALIGN(memory_management_struct.e820[i].address);
		end   = ((memory_management_struct.e820[i].address + memory_management_struct.e820[i].length) >> PAGE_2M_SHIFT) << PAGE_2M_SHIFT;
		if(end <= start)
			continue;
		TotalMem += (end - start) >> PAGE_2M_SHIFT;
	}
	
	color_printk(ORANGE,BLACK,"OS Can Used Total 2M PAGEs:%#010x=%010d\n",TotalMem,TotalMem);
}
