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
}
