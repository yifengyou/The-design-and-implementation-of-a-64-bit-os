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

	color_printk(ORANGE,BLACK,"4.memory_management_struct.bits_map:%#018lx\tmemory_management_struct.bits_map+1:%#018lx\tmemory_management_struct.bits_map+2:%#018lx\tzone_struct->page_using_count:%d\tzone_struct->page_free_count:%d\n",*memory_management_struct.bits_map,*(memory_management_struct.bits_map+1),*(memory_management_struct.bits_map+2),memory_management_struct.zones_struct->page_using_count,memory_management_struct.zones_struct->page_free_count);

	color_printk(WHITE,BLACK,"kmalloc test\n");
	for(i = 0;i< 16;i++)
	{
		color_printk(RED,BLACK,"size:%#010x\t",kmalloc_cache_size[i].size);
		color_printk(RED,BLACK,"color_map(before):%#018lx\t",*kmalloc_cache_size[i].cache_pool->color_map);
		tmp = kmalloc(kmalloc_cache_size[i].size,0);
		if(tmp == NULL)
			color_printk(RED,BLACK,"kmalloc size:%#010x ERROR\n",kmalloc_cache_size[i].size);
		color_printk(RED,BLACK,"color_map(middle):%#018lx\t",*kmalloc_cache_size[i].cache_pool->color_map);
		kfree(tmp);
		color_printk(RED,BLACK,"color_map(after):%#018lx\n",*kmalloc_cache_size[i].cache_pool->color_map);
	}

	kmalloc(kmalloc_cache_size[15].size,0);
	kmalloc(kmalloc_cache_size[15].size,0);
	kmalloc(kmalloc_cache_size[15].size,0);
	kmalloc(kmalloc_cache_size[15].size,0);
	kmalloc(kmalloc_cache_size[15].size,0);
	kmalloc(kmalloc_cache_size[15].size,0);
	kmalloc(kmalloc_cache_size[15].size,0);


	color_printk(RED,BLACK,"color_map(0):%#018lx,%#018lx\n",kmalloc_cache_size[15].cache_pool->color_map,*kmalloc_cache_size[15].cache_pool->color_map);
	slab = container_of(list_next(&kmalloc_cache_size[15].cache_pool->list),struct Slab,list);
	color_printk(RED,BLACK,"color_map(1):%#018lx,%#018lx\n",slab->color_map,*slab->color_map);
	slab = container_of(list_next(&slab->list),struct Slab,list);
	color_printk(RED,BLACK,"color_map(2):%#018lx,%#018lx\n",slab->color_map,*slab->color_map);
	slab = container_of(list_next(&slab->list),struct Slab,list);
	color_printk(RED,BLACK,"color_map(3):%#018lx,%#018lx\n",slab->color_map,*slab->color_map);
	
//	color_printk(RED,BLACK,"task_init \n");
//	task_init();

	while(1)
		;
}
