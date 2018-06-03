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
	struct Memory_E820_Formate *p = NULL;	
	
	color_printk(BLUE,BLACK,"Display Physics Address MAP,Type(1:RAM,2:ROM or Reserved,3:ACPI Reclaim Memory,4:ACPI NVS Memory,Others:Undefine)\n");
	p = (struct Memory_E820_Formate *)0xffff800000007e00;

	for(i = 0;i < 32;i++)
	{
		color_printk(ORANGE,BLACK,"Address:%#010x,%08x\tLength:%#010x,%08x\tType:%#010x\n",p->address2,p->address1,p->length2,p->length1,p->type);
		unsigned long tmp = 0;
		if(p->type == 1)
		{
			tmp = p->length2;
			TotalMem +=  p->length1;
			TotalMem +=  tmp  << 32;
		}

		p++;
		if(p->type > 4)
			break;		
	}

	color_printk(ORANGE,BLACK,"OS Can Used Total RAM:%#018lx\n",TotalMem);
}
