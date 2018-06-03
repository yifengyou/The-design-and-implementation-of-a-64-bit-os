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

#include "lib.h"
#include "printk.h"
#include "gate.h"
#include "trap.h"
#include "memory.h"
#include "task.h"
#include "cpu.h"

/*
		static var 
*/

struct Global_Memory_Descriptor memory_management_struct = {{0},0};

void Start_Kernel(void)
{
	int *addr = (int *)0xffff800003000000;
	int i;
	struct Page * page = NULL;
	void * tmp = NULL;
	struct Slab *slab = NULL;

	Pos.XResolution = 1024;
	Pos.YResolution = 768;

	Pos.XPosition = 0;
	Pos.YPosition = 0;

	Pos.XCharSize = 8;
	Pos.YCharSize = 16;

	Pos.FB_addr = (int *)0xffff800003000000;
	Pos.FB_length = (Pos.XResolution * Pos.YResolution * 4 + PAGE_4K_SIZE - 1) & PAGE_4K_MASK;

	load_TR(10);

	set_tss64(_stack_start, _stack_start, _stack_start, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00);

	sys_vector_init();

	init_cpu();
	
	memory_management_struct.start_code = (unsigned long)& _text;
	memory_management_struct.end_code   = (unsigned long)& _etext;
	memory_management_struct.end_data   = (unsigned long)& _edata;
	memory_management_struct.end_brk    = (unsigned long)& _end;

	color_printk(RED,BLACK,"memory init \n");
	init_memory();

	color_printk(RED,BLACK,"interrupt init \n");
	init_interrupt();

	color_printk(RED,BLACK,"slab init \n");
	slab_init();

	page = alloc_pages(ZONE_NORMAL,63, 0);
	page = alloc_pages(ZONE_NORMAL,63, 0);

	color_printk(ORANGE,BLACK,"4.memory_management_struct.bits_map:%#018lx\tmemory_management_struct.bits_map+1:%#018lx\tmemory_management_struct.bits_map+2:%#018lx\tzone_struct->page_using_count:%d\tzone_struct->page_free_count:%d\n",*memory_management_struct.bits_map,*(memory_management_struct.bits_map+1),*(memory_management_struct.bits_map+2),memory_management_struct.zones_struct->page_using_count,memory_management_struct.zones_struct->page_free_count);

	for(i = 80;i <= 85;i++)
	{
		color_printk(INDIGO,BLACK,"page%03d attribute:%#018lx address:%#018lx\t",i,(memory_management_struct.pages_struct + i)->attribute,(memory_management_struct.pages_struct + i)->PHY_address);
		i++;
		color_printk(INDIGO,BLACK,"page%03d attribute:%#018lx address:%#018lx\n",i,(memory_management_struct.pages_struct + i)->attribute,(memory_management_struct.pages_struct + i)->PHY_address);
	}

	for(i = 140;i <= 145;i++)
	{
		color_printk(INDIGO,BLACK,"page%03d attribute:%#018lx address:%#018lx\t",i,(memory_management_struct.pages_struct + i)->attribute,(memory_management_struct.pages_struct + i)->PHY_address);
		i++;
		color_printk(INDIGO,BLACK,"page%03d attribute:%#018lx address:%#018lx\n",i,(memory_management_struct.pages_struct + i)->attribute,(memory_management_struct.pages_struct + i)->PHY_address);
	}

	free_pages(page,1);
	
	color_printk(ORANGE,BLACK,"5.memory_management_struct.bits_map:%#018lx\tmemory_management_struct.bits_map+1:%#018lx\tmemory_management_struct.bits_map+2:%#018lx\tzone_struct->page_using_count:%d\tzone_struct->page_free_count:%d\n",*memory_management_struct.bits_map,*(memory_management_struct.bits_map+1),*(memory_management_struct.bits_map+2),memory_management_struct.zones_struct->page_using_count,memory_management_struct.zones_struct->page_free_count);

	for(i = 75;i <= 85;i++)
	{
		color_printk(INDIGO,BLACK,"page%03d attribute:%#018lx address:%#018lx\t",i,(memory_management_struct.pages_struct + i)->attribute,(memory_management_struct.pages_struct + i)->PHY_address);
		i++;
		color_printk(INDIGO,BLACK,"page%03d attribute:%#018lx address:%#018lx\n",i,(memory_management_struct.pages_struct + i)->attribute,(memory_management_struct.pages_struct + i)->PHY_address);
	}
	
	page = alloc_pages(ZONE_UNMAPED,63, 0);

	color_printk(ORANGE,BLACK,"6.memory_management_struct.bits_map:%#018lx\tmemory_management_struct.bits_map+1:%#018lx\tzone_struct->page_using_count:%d\tzone_struct->page_free_count:%d\n",*(memory_management_struct.bits_map + (page->PHY_address >> PAGE_2M_SHIFT >> 6)) , *(memory_management_struct.bits_map + 1 + (page->PHY_address >> PAGE_2M_SHIFT >> 6)) ,(memory_management_struct.zones_struct + ZONE_UNMAPED_INDEX)->page_using_count,(memory_management_struct.zones_struct + ZONE_UNMAPED_INDEX)->page_free_count );
	
	free_pages(page,1);

	color_printk(ORANGE,BLACK,"7.memory_management_struct.bits_map:%#018lx\tmemory_management_struct.bits_map+1:%#018lx\tzone_struct->page_using_count:%d\tzone_struct->page_free_count:%d\n",*(memory_management_struct.bits_map + (page->PHY_address >> PAGE_2M_SHIFT >> 6)) , *(memory_management_struct.bits_map + 1 + (page->PHY_address >> PAGE_2M_SHIFT >> 6)) ,(memory_management_struct.zones_struct + ZONE_UNMAPED_INDEX)->page_using_count,(memory_management_struct.zones_struct + ZONE_UNMAPED_INDEX)->page_free_count );

	//	color_printk(RED,BLACK,"task_init \n");
	//	task_init();

	while(1)
		;
}
