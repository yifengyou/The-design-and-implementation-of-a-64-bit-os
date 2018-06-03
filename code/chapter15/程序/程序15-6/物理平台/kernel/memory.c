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
#include "printk.h"
#include "lib.h"
#include "errno.h"


struct Global_Memory_Descriptor memory_management_struct = {{0},0};

struct Slab_cache kmalloc_cache_size[16] = 
{
	{32	,0	,0	,NULL	,NULL	,NULL	,NULL},
	{64	,0	,0	,NULL	,NULL	,NULL	,NULL},
	{128	,0	,0	,NULL	,NULL	,NULL	,NULL},
	{256	,0	,0	,NULL	,NULL	,NULL	,NULL},
	{512	,0	,0	,NULL	,NULL	,NULL	,NULL},
	{1024	,0	,0	,NULL	,NULL	,NULL	,NULL},			//1KB
	{2048	,0	,0	,NULL	,NULL	,NULL	,NULL},
	{4096	,0	,0	,NULL	,NULL	,NULL	,NULL},			//4KB
	{8192	,0	,0	,NULL	,NULL	,NULL	,NULL},
	{16384	,0	,0	,NULL	,NULL	,NULL	,NULL},
	{32768	,0	,0	,NULL	,NULL	,NULL	,NULL},
	{65536	,0	,0	,NULL	,NULL	,NULL	,NULL},			//64KB
	{131072	,0	,0	,NULL	,NULL	,NULL	,NULL},			//128KB
	{262144	,0	,0	,NULL	,NULL	,NULL	,NULL},
	{524288	,0	,0	,NULL	,NULL	,NULL	,NULL},
	{1048576,0	,0	,NULL	,NULL	,NULL	,NULL},			//1MB
};


unsigned long page_init(struct Page * page,unsigned long flags)
{
	page->attribute |= flags;
	
	if(!page->reference_count || (page->attribute & PG_Shared))
	{
		page->reference_count++;
		page->zone_struct->total_pages_link++;		
	}	
	
	return 1;
}


unsigned long page_clean(struct Page * page)
{
	page->reference_count--;
	page->zone_struct->total_pages_link--;

	if(!page->reference_count)
	{
		page->attribute &= PG_PTable_Maped;
	}
	
	return 1;
}

unsigned long get_page_attribute(struct Page * page)
{
	if(page == NULL)
	{
		color_printk(RED,BLACK,"get_page_attribute() ERROR: page == NULL\n");
		return 0;
	}
	else
		return page->attribute;
}

unsigned long set_page_attribute(struct Page * page,unsigned long flags)
{
	if(page == NULL)
	{
		color_printk(RED,BLACK,"set_page_attribute() ERROR: page == NULL\n");
		return 0;
	}
	else
	{
		page->attribute = flags;
		return 1;
	}
}

void init_memory()
{
	int i,j;
	unsigned long TotalMem = 0 ;
	struct E820 *p = NULL;	
	unsigned long * tmp = NULL;
	
	color_printk(BLUE,BLACK,"Display Physics Address MAP,Type(1:RAM,2:ROM or Reserved,3:ACPI Reclaim Memory,4:ACPI NVS Memory,Others:Undefine)\n");
	p = (struct E820 *)0xffff800000007e00;

	for(i = 0;i < 32;i++)
	{
		if(p->type == 1)
		color_printk(ORANGE,BLACK,"Address:%#018lx\tLength:%#018lx\tType:%#010x\n",p->address,p->length,p->type);
		if(p->type == 1)
			TotalMem +=  p->length;

		memory_management_struct.e820[i].address = p->address;

		memory_management_struct.e820[i].length	 = p->length;

		memory_management_struct.e820[i].type	 = p->type;
		
		memory_management_struct.e820_length = i;

		p++;
		if(p->type > 4 || p->length == 0 || p->type < 1)
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
	
	color_printk(ORANGE,BLACK,"OS Can Used Total 2M PAGEs:%#018lx=%018ld\n",TotalMem,TotalMem);

	TotalMem = memory_management_struct.e820[memory_management_struct.e820_length].address + memory_management_struct.e820[memory_management_struct.e820_length].length;

	//bits map construction init

	memory_management_struct.bits_map = (unsigned long *)((memory_management_struct.start_brk + PAGE_4K_SIZE - 1) & PAGE_4K_MASK);

	memory_management_struct.bits_size = TotalMem >> PAGE_2M_SHIFT;

	memory_management_struct.bits_length = (((unsigned long)(TotalMem >> PAGE_2M_SHIFT) + sizeof(long) * 8 - 1) / 8) & ( ~ (sizeof(long) - 1));

	memset(memory_management_struct.bits_map,0xff,memory_management_struct.bits_length);		//init bits map memory

	//pages construction init

	memory_management_struct.pages_struct = (struct Page *)(((unsigned long)memory_management_struct.bits_map + memory_management_struct.bits_length + PAGE_4K_SIZE - 1) & PAGE_4K_MASK);

	memory_management_struct.pages_size = TotalMem >> PAGE_2M_SHIFT;

	memory_management_struct.pages_length = ((TotalMem >> PAGE_2M_SHIFT) * sizeof(struct Page) + sizeof(long) - 1) & ( ~ (sizeof(long) - 1));

	memset(memory_management_struct.pages_struct,0x00,memory_management_struct.pages_length);	//init pages memory

	//zones construction init

	memory_management_struct.zones_struct = (struct Zone *)(((unsigned long)memory_management_struct.pages_struct + memory_management_struct.pages_length + PAGE_4K_SIZE - 1) & PAGE_4K_MASK);

	memory_management_struct.zones_size   = 0;

	memory_management_struct.zones_length = (5 * sizeof(struct Zone) + sizeof(long) - 1) & (~(sizeof(long) - 1));

	memset(memory_management_struct.zones_struct,0x00,memory_management_struct.zones_length);	//init zones memory

	for(i = 0;i <= memory_management_struct.e820_length;i++)
	{
		unsigned long start,end;
		struct Zone * z;
		struct Page * p;

		if(memory_management_struct.e820[i].type != 1)
			continue;
		start = PAGE_2M_ALIGN(memory_management_struct.e820[i].address);
		end   = ((memory_management_struct.e820[i].address + memory_management_struct.e820[i].length) >> PAGE_2M_SHIFT) << PAGE_2M_SHIFT;
		if(end <= start)
			continue;
		
		//zone init

		z = memory_management_struct.zones_struct + memory_management_struct.zones_size;
		memory_management_struct.zones_size++;

		z->zone_start_address = start;
		z->zone_end_address = end;
		z->zone_length = end - start;

		z->page_using_count = 0;
		z->page_free_count = (end - start) >> PAGE_2M_SHIFT;

		z->total_pages_link = 0;

		z->attribute = 0;
		z->GMD_struct = &memory_management_struct;

		z->pages_length = (end - start) >> PAGE_2M_SHIFT;
		z->pages_group =  (struct Page *)(memory_management_struct.pages_struct + (start >> PAGE_2M_SHIFT));

		//page init
		p = z->pages_group;
		for(j = 0;j < z->pages_length; j++ , p++)
		{
			p->zone_struct = z;
			p->PHY_address = start + PAGE_2M_SIZE * j;
			p->attribute = 0;

			p->reference_count = 0;

			p->age = 0;

			*(memory_management_struct.bits_map + ((p->PHY_address >> PAGE_2M_SHIFT) >> 6)) ^= 1UL << (p->PHY_address >> PAGE_2M_SHIFT) % 64;
		}		
	}

	/////////////init address 0 to page struct 0; because the memory_management_struct.e820[0].type != 1
	
	memory_management_struct.pages_struct->zone_struct = memory_management_struct.zones_struct;

	memory_management_struct.pages_struct->PHY_address = 0UL;

	set_page_attribute(memory_management_struct.pages_struct,PG_PTable_Maped | PG_Kernel_Init | PG_Kernel);
	memory_management_struct.pages_struct->reference_count = 1;
	memory_management_struct.pages_struct->age = 0;
	
	/////////////

	memory_management_struct.zones_length = (memory_management_struct.zones_size * sizeof(struct Zone) + sizeof(long) - 1) & ( ~ (sizeof(long) - 1));

	color_printk(ORANGE,BLACK,"bits_map:%#018lx,bits_size:%#018lx,bits_length:%#018lx\n",memory_management_struct.bits_map,memory_management_struct.bits_size,memory_management_struct.bits_length);

	color_printk(ORANGE,BLACK,"pages_struct:%#018lx,pages_size:%#018lx,pages_length:%#018lx\n",memory_management_struct.pages_struct,memory_management_struct.pages_size,memory_management_struct.pages_length);

	color_printk(ORANGE,BLACK,"zones_struct:%#018lx,zones_size:%#018lx,zones_length:%#018lx\n",memory_management_struct.zones_struct,memory_management_struct.zones_size,memory_management_struct.zones_length);

	ZONE_DMA_INDEX = 0;	
	ZONE_NORMAL_INDEX = 0;	
	ZONE_UNMAPED_INDEX = 0;
	
	for(i = 0;i < memory_management_struct.zones_size;i++)	
	{
		struct Zone * z = memory_management_struct.zones_struct + i;
		color_printk(ORANGE,BLACK,"zone_start_address:%#018lx,zone_end_address:%#018lx,zone_length:%#018lx,pages_group:%#018lx,pages_length:%#018lx\n",z->zone_start_address,z->zone_end_address,z->zone_length,z->pages_group,z->pages_length);

		if(z->zone_start_address >= 0x100000000 && !ZONE_UNMAPED_INDEX)
			ZONE_UNMAPED_INDEX = i;
	}
	
	color_printk(ORANGE,BLACK,"ZONE_DMA_INDEX:%d\tZONE_NORMAL_INDEX:%d\tZONE_UNMAPED_INDEX:%d\n",ZONE_DMA_INDEX,ZONE_NORMAL_INDEX,ZONE_UNMAPED_INDEX);
	
	memory_management_struct.end_of_struct = (unsigned long)((unsigned long)memory_management_struct.zones_struct + memory_management_struct.zones_length + sizeof(long) * 32) & ( ~ (sizeof(long) - 1));	////need a blank to separate memory_management_struct

	color_printk(ORANGE,BLACK,"start_code:%#018lx,end_code:%#018lx,end_data:%#018lx,start_brk:%#018lx,end_of_struct:%#018lx\n",memory_management_struct.start_code,memory_management_struct.end_code,memory_management_struct.end_data,memory_management_struct.start_brk, memory_management_struct.end_of_struct);
	
	i = Virt_To_Phy(memory_management_struct.end_of_struct) >> PAGE_2M_SHIFT;

	for(j = 1;j <= i;j++)
	{
		struct Page * tmp_page =  memory_management_struct.pages_struct + j;
		page_init(tmp_page,PG_PTable_Maped | PG_Kernel_Init | PG_Kernel);
		*(memory_management_struct.bits_map + ((tmp_page->PHY_address >> PAGE_2M_SHIFT) >> 6)) |= 1UL << (tmp_page->PHY_address >> PAGE_2M_SHIFT) % 64;
		tmp_page->zone_struct->page_using_count++;
		tmp_page->zone_struct->page_free_count--;
	}

	tmp = Get_gdt();

	color_printk(INDIGO,BLACK,"tmp\t:%#018lx\n",tmp);
	color_printk(INDIGO,BLACK,"*tmp\t:%#018lx\n",*Phy_To_Virt(tmp) & (~0xff));
	color_printk(PURPLE,BLACK,"**tmp\t:%#018lx\n",*Phy_To_Virt(*Phy_To_Virt(tmp) & (~0xff)) & (~0xff));

	color_printk(ORANGE,BLACK,"1.memory_management_struct.bits_map:%#018lx\tzone_struct->page_using_count:%d\tzone_struct->page_free_count:%d\n",*memory_management_struct.bits_map,memory_management_struct.zones_struct->page_using_count,memory_management_struct.zones_struct->page_free_count);

//	for(i = 0;i < 10;i++)
//		*(Phy_To_Virt(Global_CR3)  + i) = 0UL;

	flush_tlb();
}

/*

	number: number < 64
	zone_select: zone select from dma , mapped in  pagetable , unmapped in pagetable
	page_flags: struct Page flages

*/

struct Page * alloc_pages(int zone_select,int number,unsigned long page_flags)
{
	int i;
	unsigned long page = 0;
	unsigned long attribute = 0;

	int zone_start = 0;
	int zone_end = 0;
	
	if(number >= 64 || number <= 0)
	{
		color_printk(RED,BLACK,"alloc_pages() ERROR: number is invalid\n");
		return NULL;		
	}

	switch(zone_select)
	{
		case ZONE_DMA:
				zone_start = 0;
				zone_end = ZONE_DMA_INDEX;
				attribute = PG_PTable_Maped;
			break;

		case ZONE_NORMAL:
				zone_start = ZONE_DMA_INDEX;
				zone_end = ZONE_NORMAL_INDEX;
				attribute = PG_PTable_Maped;
			break;

		case ZONE_UNMAPED:
				zone_start = ZONE_UNMAPED_INDEX;
				zone_end = memory_management_struct.zones_size - 1;
				attribute = 0;
			break;

		default:
			color_printk(RED,BLACK,"alloc_pages() ERROR: zone_select index is invalid\n");
			return NULL;
			break;
	}

	for(i = zone_start; i <= zone_end; i++)
	{
		struct Zone * z;
		unsigned long j;
		unsigned long start,end;
		unsigned long tmp;

		if((memory_management_struct.zones_struct + i)->page_free_count < number)
			continue;

		z = memory_management_struct.zones_struct + i;
		start = z->zone_start_address >> PAGE_2M_SHIFT;
		end = z->zone_end_address >> PAGE_2M_SHIFT;

		tmp = 64 - start % 64;

		for(j = start;j < end;j += j % 64 ? tmp : 64)
		{
			unsigned long * p = memory_management_struct.bits_map + (j >> 6);
			unsigned long k = 0;
			unsigned long shift = j % 64;
			
			unsigned long num = (1UL << number) - 1;
			
			for(k = shift;k < 64;k++)
			{
				if( !( (k ? ((*p >> k) | (*(p + 1) << (64 - k))) : *p) & (num) ) )
				{
					unsigned long	l;
					page = j + k - shift;
					for(l = 0;l < number;l++)
					{
						struct Page * pageptr = memory_management_struct.pages_struct + page + l;

						*(memory_management_struct.bits_map + ((pageptr->PHY_address >> PAGE_2M_SHIFT) >> 6)) |= 1UL << (pageptr->PHY_address >> PAGE_2M_SHIFT) % 64;
						z->page_using_count++;
						z->page_free_count--;
						pageptr->attribute = attribute;
					}
					goto find_free_pages;
				}
			}
		}
	}

	color_printk(RED,BLACK,"alloc_pages() ERROR: no page can alloc\n");
	return NULL;

find_free_pages:

	return (struct Page *)(memory_management_struct.pages_struct + page);
}

/*

	page: free page start from this pointer
	number: number < 64

*/

void free_pages(struct Page * page,int number)
{	
	int i = 0;
	
	if(page == NULL)
	{
		color_printk(RED,BLACK,"free_pages() ERROR: page is invalid\n");
		return ;
	}	

	if(number >= 64 || number <= 0)
	{
		color_printk(RED,BLACK,"free_pages() ERROR: number is invalid\n");
		return ;	
	}
	
	for(i = 0;i<number;i++,page++)
	{
		*(memory_management_struct.bits_map + ((page->PHY_address >> PAGE_2M_SHIFT) >> 6)) &= ~(1UL << (page->PHY_address >> PAGE_2M_SHIFT) % 64);
		page->zone_struct->page_using_count--;
		page->zone_struct->page_free_count++;
		page->attribute = 0;
	}
}

/*
	return virtual kernel address
	gfp_flages: the condition of get memory 
*/

void * kmalloc(unsigned long size,unsigned long gfp_flages)
{
	int i,j;
	struct Slab * slab = NULL;
	if(size > 1048576)
	{
		color_printk(RED,BLACK,"kmalloc() ERROR: kmalloc size too long:%08d\n",size);
		return NULL;
	}
	for(i = 0;i < 16;i++)
		if(kmalloc_cache_size[i].size >= size)
			break;
	slab = kmalloc_cache_size[i].cache_pool;

	if(kmalloc_cache_size[i].total_free != 0)
	{
		do
		{
			if(slab->free_count == 0)
				slab = container_of(list_next(&slab->list),struct Slab,list);
			else
				break;
		}while(slab != kmalloc_cache_size[i].cache_pool);	
	}
	else
	{
		slab = kmalloc_create(kmalloc_cache_size[i].size);
		
		if(slab == NULL)
		{
			color_printk(BLUE,BLACK,"kmalloc()->kmalloc_create()=>slab == NULL\n");
			return NULL;
		}
		
		kmalloc_cache_size[i].total_free += slab->color_count;

		color_printk(BLUE,BLACK,"kmalloc()->kmalloc_create()<=size:%#010x\n",kmalloc_cache_size[i].size);///////
		
		list_add_to_before(&kmalloc_cache_size[i].cache_pool->list,&slab->list);
	}

	for(j = 0;j < slab->color_count;j++)
	{
		if(*(slab->color_map + (j >> 6)) == 0xffffffffffffffffUL)
		{
			j += 63;
			continue;
		}
			
		if( (*(slab->color_map + (j >> 6)) & (1UL << (j % 64))) == 0 )
		{
			*(slab->color_map + (j >> 6)) |= 1UL << (j % 64);
			slab->using_count++;
			slab->free_count--;

			kmalloc_cache_size[i].total_free--;
			kmalloc_cache_size[i].total_using++;

			return (void *)((char *)slab->Vaddress + kmalloc_cache_size[i].size * j);
		}
	}

	color_printk(BLUE,BLACK,"kmalloc() ERROR: no memory can alloc\n");
	return NULL;
}

/*

*/

struct Slab * kmalloc_create(unsigned long size)
{
	int i;
	struct Slab * slab = NULL;
	struct Page * page = NULL;
	unsigned long * vaddresss = NULL;
	long structsize = 0;

	page = alloc_pages(ZONE_NORMAL,1, 0);
	
	if(page == NULL)
	{
		color_printk(RED,BLACK,"kmalloc_create()->alloc_pages()=>page == NULL\n");
		return NULL;
	}
	
	page_init(page,PG_Kernel);

	switch(size)
	{
		////////////////////slab + map in 2M page

		case 32:
		case 64:
		case 128:
		case 256:
		case 512:

			vaddresss = Phy_To_Virt(page->PHY_address);
			structsize = sizeof(struct Slab) + PAGE_2M_SIZE / size / 8;

			slab = (struct Slab *)((unsigned char *)vaddresss + PAGE_2M_SIZE - structsize);
			slab->color_map = (unsigned long *)((unsigned char *)slab + sizeof(struct Slab));

			slab->free_count = (PAGE_2M_SIZE - (PAGE_2M_SIZE / size / 8) - sizeof(struct Slab)) / size;
			slab->using_count = 0;
			slab->color_count = slab->free_count;
			slab->Vaddress = vaddresss;
			slab->page = page;
			list_init(&slab->list);

			slab->color_length = ((slab->color_count + sizeof(unsigned long) * 8 - 1) >> 6) << 3;
			memset(slab->color_map,0xff,slab->color_length);

			for(i = 0;i < slab->color_count;i++)
				*(slab->color_map + (i >> 6)) ^= 1UL << i % 64;

			break;

		///////////////////kmalloc slab and map,not in 2M page anymore

		case 1024:		//1KB
		case 2048:
		case 4096:		//4KB
		case 8192:
		case 16384:

		//////////////////color_map is a very short buffer.

		case 32768:
		case 65536:
		case 131072:		//128KB
		case 262144:
		case 524288:
		case 1048576:		//1MB

			slab = (struct Slab *)kmalloc(sizeof(struct Slab),0);

			slab->free_count = PAGE_2M_SIZE / size;
			slab->using_count = 0;
			slab->color_count = slab->free_count;

			slab->color_length = ((slab->color_count + sizeof(unsigned long) * 8 - 1) >> 6) << 3;

			slab->color_map = (unsigned long *)kmalloc(slab->color_length,0);
			memset(slab->color_map,0xff,slab->color_length);

			slab->Vaddress = Phy_To_Virt(page->PHY_address);
			slab->page = page;
			list_init(&slab->list);

			for(i = 0;i < slab->color_count;i++)
				*(slab->color_map + (i >> 6)) ^= 1UL << i % 64;

			break;

		default:

			color_printk(RED,BLACK,"kmalloc_create() ERROR: wrong size:%08d\n",size);
			free_pages(page,1);
			
			return NULL;
	}	
	
	return slab;
}

/*

*/

unsigned long kfree(void * address)
{
	int i;
	int index;
	struct Slab * slab = NULL;
	void * page_base_address = (void *)((unsigned long)address & PAGE_2M_MASK);

	for(i = 0;i < 16;i++)
	{
		slab = kmalloc_cache_size[i].cache_pool;
		do
		{
			if(slab->Vaddress == page_base_address)
			{
				index = (address - slab->Vaddress) / kmalloc_cache_size[i].size;

				*(slab->color_map + (index >> 6)) ^= 1UL << index % 64;

				slab->free_count++;
				slab->using_count--;

				kmalloc_cache_size[i].total_free++;
				kmalloc_cache_size[i].total_using--;

				if((slab->using_count == 0) && (kmalloc_cache_size[i].total_free >= slab->color_count * 3 / 2) && (kmalloc_cache_size[i].cache_pool != slab))
				{
					switch(kmalloc_cache_size[i].size)
					{
						////////////////////slab + map in 2M page
				
						case 32:
						case 64:
						case 128:
						case 256:	
						case 512:
							list_del(&slab->list);
							kmalloc_cache_size[i].total_free -= slab->color_count;

							page_clean(slab->page);
							free_pages(slab->page,1);
							break;
				
						default:
							list_del(&slab->list);
							kmalloc_cache_size[i].total_free -= slab->color_count;

							kfree(slab->color_map);

							page_clean(slab->page);
							free_pages(slab->page,1);
							kfree(slab);
							break;
					}
 
				}

				return 1;
			}
			else
				slab = container_of(list_next(&slab->list),struct Slab,list);				

		}while(slab != kmalloc_cache_size[i].cache_pool);
	
	}
	
	color_printk(RED,BLACK,"kfree() ERROR: can`t free memory\n");
	
	return 0;
}

/*

*/

struct Slab_cache * slab_create(unsigned long size,void *(* constructor)(void * Vaddress,unsigned long arg),void *(* destructor)(void * Vaddress,unsigned long arg),unsigned long arg)
{
	struct Slab_cache * slab_cache = NULL;

	slab_cache = (struct Slab_cache *)kmalloc(sizeof(struct Slab_cache),0);
	
	if(slab_cache == NULL)
	{
		color_printk(RED,BLACK,"slab_create()->kmalloc()=>slab_cache == NULL\n");
		return NULL;
	}
	
	memset(slab_cache,0,sizeof(struct Slab_cache));

	slab_cache->size = SIZEOF_LONG_ALIGN(size);
	slab_cache->total_using = 0;
	slab_cache->total_free = 0;
	slab_cache->cache_pool = (struct Slab *)kmalloc(sizeof(struct Slab),0);
	
	if(slab_cache->cache_pool == NULL)
	{
		color_printk(RED,BLACK,"slab_create()->kmalloc()=>slab_cache->cache_pool == NULL\n");
		kfree(slab_cache);
		return NULL;
	}
	
	memset(slab_cache->cache_pool,0,sizeof(struct Slab));

	slab_cache->cache_dma_pool = NULL;
	slab_cache->constructor = constructor;
	slab_cache->destructor = destructor;

	list_init(&slab_cache->cache_pool->list);

	slab_cache->cache_pool->page = alloc_pages(ZONE_NORMAL,1,0);
	
	if(slab_cache->cache_pool->page == NULL)
	{
		color_printk(RED,BLACK,"slab_create()->alloc_pages()=>slab_cache->cache_pool->page == NULL\n");
		kfree(slab_cache->cache_pool);
		kfree(slab_cache);
		return NULL;
	}
	
	page_init(slab_cache->cache_pool->page,PG_Kernel);

	slab_cache->cache_pool->using_count = PAGE_2M_SIZE/slab_cache->size;

	slab_cache->cache_pool->free_count = slab_cache->cache_pool->using_count;

	slab_cache->total_free = slab_cache->cache_pool->free_count;

	slab_cache->cache_pool->Vaddress = Phy_To_Virt(slab_cache->cache_pool->page->PHY_address);

	slab_cache->cache_pool->color_count = slab_cache->cache_pool->free_count;

	slab_cache->cache_pool->color_length = ((slab_cache->cache_pool->color_count + sizeof(unsigned long) * 8 - 1) >> 6) << 3;

	slab_cache->cache_pool->color_map = (unsigned long *)kmalloc(slab_cache->cache_pool->color_length,0);

	if(slab_cache->cache_pool->color_map == NULL)
	{
		color_printk(RED,BLACK,"slab_create()->kmalloc()=>slab_cache->cache_pool->color_map == NULL\n");
		
		free_pages(slab_cache->cache_pool->page,1);
		kfree(slab_cache->cache_pool);
		kfree(slab_cache);
		return NULL;
	}
	
	memset(slab_cache->cache_pool->color_map,0,slab_cache->cache_pool->color_length);

	return slab_cache;
}

/*

*/

unsigned long slab_destroy(struct Slab_cache * slab_cache)
{
	struct Slab * slab_p = slab_cache->cache_pool;
	struct Slab * tmp_slab = NULL;

	if(slab_cache->total_using != 0)
	{
		color_printk(RED,BLACK,"slab_cache->total_using != 0\n");
		return 0;
	}

	while(!list_is_empty(&slab_p->list))
	{
		tmp_slab = slab_p;
		slab_p = container_of(list_next(&slab_p->list),struct Slab,list);

		list_del(&tmp_slab->list);
		kfree(tmp_slab->color_map);

		page_clean(tmp_slab->page);
		free_pages(tmp_slab->page,1);
		kfree(tmp_slab);
	}

	kfree(slab_p->color_map);

	page_clean(slab_p->page);
	free_pages(slab_p->page,1);
	kfree(slab_p);
		
	kfree(slab_cache);

	return 1;
}

/*

*/

void * slab_malloc(struct Slab_cache * slab_cache,unsigned long arg)
{
	struct Slab * slab_p = slab_cache->cache_pool;
	struct Slab * tmp_slab = NULL;
	int j = 0;

	if(slab_cache->total_free == 0)
	{
		tmp_slab = (struct Slab *)kmalloc(sizeof(struct Slab),0);
	
		if(tmp_slab == NULL)
		{
			color_printk(RED,BLACK,"slab_malloc()->kmalloc()=>tmp_slab == NULL\n");
			return NULL;
		}
	
		memset(tmp_slab,0,sizeof(struct Slab));

		list_init(&tmp_slab->list);

		tmp_slab->page = alloc_pages(ZONE_NORMAL,1,0);
	
		if(tmp_slab->page == NULL)
		{
			color_printk(RED,BLACK,"slab_malloc()->alloc_pages()=>tmp_slab->page == NULL\n");
			kfree(tmp_slab);
			return NULL;
		}
	
		page_init(tmp_slab->page,PG_Kernel);

		tmp_slab->using_count = PAGE_2M_SIZE/slab_cache->size;
		tmp_slab->free_count = tmp_slab->using_count;
		tmp_slab->Vaddress = Phy_To_Virt(tmp_slab->page->PHY_address);

		tmp_slab->color_count = tmp_slab->free_count;
		tmp_slab->color_length = ((tmp_slab->color_count + sizeof(unsigned long) * 8 - 1) >> 6) << 3;
		tmp_slab->color_map = (unsigned long *)kmalloc(tmp_slab->color_length,0);

		if(tmp_slab->color_map == NULL)
		{
			color_printk(RED,BLACK,"slab_malloc()->kmalloc()=>tmp_slab->color_map == NULL\n");
			free_pages(tmp_slab->page,1);
			kfree(tmp_slab);
			return NULL;
		}
	
		memset(tmp_slab->color_map,0,tmp_slab->color_length);	

		list_add_to_behind(&slab_cache->cache_pool->list,&tmp_slab->list);

		slab_cache->total_free  += tmp_slab->color_count;	

		for(j = 0;j < tmp_slab->color_count;j++)
		{
			if( (*(tmp_slab->color_map + (j >> 6)) & (1UL << (j % 64))) == 0 )
			{
				*(tmp_slab->color_map + (j >> 6)) |= 1UL << (j % 64);
			
				tmp_slab->using_count++;
				tmp_slab->free_count--;

				slab_cache->total_using++;
				slab_cache->total_free--;
				
				if(slab_cache->constructor != NULL)
				{
					return slab_cache->constructor((char *)tmp_slab->Vaddress + slab_cache->size * j,arg);
				}
				else
				{			
					return (void *)((char *)tmp_slab->Vaddress + slab_cache->size * j);
				}		
			}
		}
	}
	else
	{
		do
		{
			if(slab_p->free_count == 0)
			{
				slab_p = container_of(list_next(&slab_p->list),struct Slab,list);
				continue;
			}
		
			for(j = 0;j < slab_p->color_count;j++)
			{
				
				if(*(slab_p->color_map + (j >> 6)) == 0xffffffffffffffffUL)
				{
					j += 63;
					continue;
				}	
				
				if( (*(slab_p->color_map + (j >> 6)) & (1UL << (j % 64))) == 0 )
				{
					*(slab_p->color_map + (j >> 6)) |= 1UL << (j % 64);
				
					slab_p->using_count++;
					slab_p->free_count--;

					slab_cache->total_using++;
					slab_cache->total_free--;
					
					if(slab_cache->constructor != NULL)
					{
						return slab_cache->constructor((char *)slab_p->Vaddress + slab_cache->size * j,arg);
					}
					else
					{
						return (void *)((char *)slab_p->Vaddress + slab_cache->size * j);
					}
				}
			}
		}while(slab_p != slab_cache->cache_pool);		
	}

	color_printk(RED,BLACK,"slab_malloc() ERROR: can`t alloc\n");
	if(tmp_slab != NULL)
	{
		list_del(&tmp_slab->list);
		kfree(tmp_slab->color_map);
		page_clean(tmp_slab->page);
		free_pages(tmp_slab->page,1);
		kfree(tmp_slab);
	}

	return NULL;
}

/*

*/

unsigned long slab_free(struct Slab_cache * slab_cache,void * address,unsigned long arg)
{
	
	struct Slab * slab_p = slab_cache->cache_pool;
	int index = 0;

	do
	{
		if(slab_p->Vaddress <= address && address < slab_p->Vaddress + PAGE_2M_SIZE)
		{
			index = (address - slab_p->Vaddress) / slab_cache->size;
			*(slab_p->color_map + (index >> 6)) ^= 1UL << index % 64;
			slab_p->free_count++;
			slab_p->using_count--;

			slab_cache->total_using--;
			slab_cache->total_free++;
			
			if(slab_cache->destructor != NULL)
			{
				slab_cache->destructor((char *)slab_p->Vaddress + slab_cache->size * index,arg);
			}

			if((slab_p->using_count == 0) && (slab_cache->total_free >= slab_p->color_count * 3 / 2))
			{
				list_del(&slab_p->list);
				slab_cache->total_free -= slab_p->color_count;

				kfree(slab_p->color_map);
				
				page_clean(slab_p->page);
				free_pages(slab_p->page,1);
				kfree(slab_p);				
			}

			return 1;
		}
		else
		{
			slab_p = container_of(list_next(&slab_p->list),struct Slab,list);
			continue;	
		}		

	}while(slab_p != slab_cache->cache_pool);

	color_printk(RED,BLACK,"slab_free() ERROR: address not in slab\n");

	return 0;
}

/*

*/

unsigned long slab_init()
{
	struct Page * page = NULL;
	unsigned long * virtual = NULL; // get a free page and set to empty page table and return the virtual address
	unsigned long i,j;

	unsigned long tmp_address = memory_management_struct.end_of_struct;

	for(i = 0;i < 16;i++) 
	{
		kmalloc_cache_size[i].cache_pool = (struct Slab *)memory_management_struct.end_of_struct;
		memory_management_struct.end_of_struct = memory_management_struct.end_of_struct + sizeof(struct Slab) + sizeof(long) * 10;

		list_init(&kmalloc_cache_size[i].cache_pool->list);	

	//////////// init sizeof struct Slab of cache size

		kmalloc_cache_size[i].cache_pool->using_count = 0;
		kmalloc_cache_size[i].cache_pool->free_count  = PAGE_2M_SIZE / kmalloc_cache_size[i].size;

		kmalloc_cache_size[i].cache_pool->color_length =((PAGE_2M_SIZE / kmalloc_cache_size[i].size + sizeof(unsigned long) * 8 - 1) >> 6) << 3;

		kmalloc_cache_size[i].cache_pool->color_count = kmalloc_cache_size[i].cache_pool->free_count;

		kmalloc_cache_size[i].cache_pool->color_map = (unsigned long *)memory_management_struct.end_of_struct;
	
		memory_management_struct.end_of_struct = (unsigned long)(memory_management_struct.end_of_struct + kmalloc_cache_size[i].cache_pool->color_length + sizeof(long) * 10) & ( ~ (sizeof(long) - 1));

		memset(kmalloc_cache_size[i].cache_pool->color_map,0xff,kmalloc_cache_size[i].cache_pool->color_length);

		for(j = 0;j < kmalloc_cache_size[i].cache_pool->color_count;j++)
			*(kmalloc_cache_size[i].cache_pool->color_map + (j >> 6)) ^= 1UL << j % 64;

		kmalloc_cache_size[i].total_free = kmalloc_cache_size[i].cache_pool->color_count;
		kmalloc_cache_size[i].total_using = 0;

	}
	
	////////////	init page for kernel code and memory management struct

	i = Virt_To_Phy(memory_management_struct.end_of_struct) >> PAGE_2M_SHIFT;

	for(j = PAGE_2M_ALIGN(Virt_To_Phy(tmp_address)) >> PAGE_2M_SHIFT;j <= i;j++)
	{
		page =  memory_management_struct.pages_struct + j;
		*(memory_management_struct.bits_map + ((page->PHY_address >> PAGE_2M_SHIFT) >> 6)) |= 1UL << (page->PHY_address >> PAGE_2M_SHIFT) % 64;
		page->zone_struct->page_using_count++;
		page->zone_struct->page_free_count--;
		page_init(page,PG_PTable_Maped | PG_Kernel_Init | PG_Kernel);
	}

	color_printk(ORANGE,BLACK,"2.memory_management_struct.bits_map:%#018lx\tzone_struct->page_using_count:%d\tzone_struct->page_free_count:%d\n",*memory_management_struct.bits_map,memory_management_struct.zones_struct->page_using_count,memory_management_struct.zones_struct->page_free_count);

	for(i = 0;i < 16;i++)
	{
		virtual = (unsigned long *)((memory_management_struct.end_of_struct + PAGE_2M_SIZE * i + PAGE_2M_SIZE - 1) & PAGE_2M_MASK);
		page = Virt_To_2M_Page(virtual);

		*(memory_management_struct.bits_map + ((page->PHY_address >> PAGE_2M_SHIFT) >> 6)) |= 1UL << (page->PHY_address >> PAGE_2M_SHIFT) % 64;
		page->zone_struct->page_using_count++;
		page->zone_struct->page_free_count--;

		page_init(page,PG_PTable_Maped | PG_Kernel_Init | PG_Kernel);

		kmalloc_cache_size[i].cache_pool->page = page;
		kmalloc_cache_size[i].cache_pool->Vaddress = virtual;
	}

	color_printk(ORANGE,BLACK,"3.memory_management_struct.bits_map:%#018lx\tzone_struct->page_using_count:%d\tzone_struct->page_free_count:%d\n",*memory_management_struct.bits_map,memory_management_struct.zones_struct->page_using_count,memory_management_struct.zones_struct->page_free_count);

	color_printk(ORANGE,BLACK,"start_code:%#018lx,end_code:%#018lx,end_data:%#018lx,start_brk:%#018lx,end_of_struct:%#018lx\n",memory_management_struct.start_code,memory_management_struct.end_code,memory_management_struct.end_data,memory_management_struct.start_brk, memory_management_struct.end_of_struct);

	return 1;
}

/*

*/

void pagetable_init()
{
	unsigned long i,j;
	unsigned long * tmp = NULL;

	tmp = (unsigned long *)(((unsigned long)Phy_To_Virt((unsigned long)Get_gdt() & (~ 0xfffUL))) + 8 * 256);
		
//	color_printk(YELLOW,BLACK,"1:%#018lx,%#018lx\t\t\n",(unsigned long)tmp,*tmp);
	tmp = Phy_To_Virt(*tmp & (~0xfffUL));

//	color_printk(YELLOW,BLACK,"2:%#018lx,%#018lx\t\t\n",(unsigned long)tmp,*tmp);
	tmp = Phy_To_Virt(*tmp & (~0xfffUL));
//	color_printk(YELLOW,BLACK,"3:%#018lx,%#018lx\t\t\n",(unsigned long)tmp,*tmp);

	for(i = 0;i < memory_management_struct.zones_size;i++)
	{
		struct Zone * z = memory_management_struct.zones_struct + i;
		struct Page * p = z->pages_group;

		if(ZONE_UNMAPED_INDEX && i == ZONE_UNMAPED_INDEX)
			break;

		for(j = 0;j < z->pages_length ; j++,p++)
		{
			
			tmp = (unsigned long *)(((unsigned long)Phy_To_Virt((unsigned long)Get_gdt() & (~ 0xfffUL))) + (((unsigned long)Phy_To_Virt(p->PHY_address) >> PAGE_GDT_SHIFT) & 0x1ff) * 8);
			
			if(*tmp == 0)
			{			
				unsigned long * virtual = kmalloc(PAGE_4K_SIZE,0);
				memset(virtual,0,PAGE_4K_SIZE);
				set_mpl4t(tmp,mk_mpl4t(Virt_To_Phy(virtual),PAGE_KERNEL_GDT));
			}

			tmp = (unsigned long *)((unsigned long)Phy_To_Virt(*tmp & (~ 0xfffUL)) + (((unsigned long)Phy_To_Virt(p->PHY_address) >> PAGE_1G_SHIFT) & 0x1ff) * 8);
			
			if(*tmp == 0)
			{
				unsigned long * virtual = kmalloc(PAGE_4K_SIZE,0);
				memset(virtual,0,PAGE_4K_SIZE);
				set_pdpt(tmp,mk_pdpt(Virt_To_Phy(virtual),PAGE_KERNEL_Dir));
			}

			tmp = (unsigned long *)((unsigned long)Phy_To_Virt(*tmp & (~ 0xfffUL)) + (((unsigned long)Phy_To_Virt(p->PHY_address) >> PAGE_2M_SHIFT) & 0x1ff) * 8);

			set_pdt(tmp,mk_pdt(p->PHY_address,PAGE_KERNEL_Page));

//			if(j % 50 == 0)
//				color_printk(GREEN,BLACK,"@:%#018lx,%#018lx\t\n",(unsigned long)tmp,*tmp);
		}
	}

	flush_tlb();
}

unsigned long do_brk(unsigned long addr,unsigned long len)
{
	unsigned long * tmp = NULL;
	unsigned long * virtual = NULL;
	struct Page * p = NULL;
	unsigned long i = 0;

	for(i = addr;i < addr + len;i += PAGE_2M_SIZE)
	{
		tmp = Phy_To_Virt((unsigned long *)((unsigned long)current->mm->pgd & (~ 0xfffUL)) + ((i >> PAGE_GDT_SHIFT) & 0x1ff));
		if(*tmp == NULL)
		{
			virtual = kmalloc(PAGE_4K_SIZE,0);
			memset(virtual,0,PAGE_4K_SIZE);
			set_mpl4t(tmp,mk_mpl4t(Virt_To_Phy(virtual),PAGE_USER_GDT));
		}

		tmp = Phy_To_Virt((unsigned long *)(*tmp & (~ 0xfffUL)) + ((i >> PAGE_1G_SHIFT) & 0x1ff));
		if(*tmp == NULL)
		{
			virtual = kmalloc(PAGE_4K_SIZE,0);
			memset(virtual,0,PAGE_4K_SIZE);
			set_pdpt(tmp,mk_pdpt(Virt_To_Phy(virtual),PAGE_USER_Dir));
		}

		tmp = Phy_To_Virt((unsigned long *)(*tmp & (~ 0xfffUL)) + ((i >> PAGE_2M_SHIFT) & 0x1ff));
		if(*tmp == NULL)
		{
			p = alloc_pages(ZONE_NORMAL,1,PG_PTable_Maped);
			if(p == NULL)
				return -ENOMEM;
			set_pdt(tmp,mk_pdt(p->PHY_address,PAGE_USER_Page));
		}
	}

	current->mm->end_brk = i;

	flush_tlb();

	return i;
}


