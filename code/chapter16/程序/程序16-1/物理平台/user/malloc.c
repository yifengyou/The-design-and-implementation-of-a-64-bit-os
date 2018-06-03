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

#include "stddef.h"
#include "limits.h"
#include "stdio.h"
#include "string.h"

static unsigned long brk_start_address = 0;
static unsigned long brk_used_address = 0;
static unsigned long brk_end_address = 0;

#define	SIZE_ALIGN	(8 * sizeof(unsigned long))

void * malloc(long size)
{
	char buf[1000];
	unsigned long address = 0;

	if(size <= 0)
	{
		printf("malloc size <= 0\n");
		return NULL;
	}

	if(brk_start_address == 0)
		brk_end_address = brk_used_address = brk_start_address = brk(0);

	if(brk_end_address <= brk_used_address + SIZE_ALIGN + size)
		brk_end_address = brk(brk_end_address + ((size + SIZE_ALIGN + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1)));

	address = brk_used_address;
	brk_used_address += size + SIZE_ALIGN;

	return (void *)address;
}


void free(void * address)
{
}



