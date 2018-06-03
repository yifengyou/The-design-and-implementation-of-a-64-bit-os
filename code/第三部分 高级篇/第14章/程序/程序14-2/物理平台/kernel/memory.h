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

#ifndef __MEMORY_H__

#define __MEMORY_H__

#include "lib.h"

//	8Bytes per cell
#define PTRS_PER_PAGE	512

/*

*/

#define PAGE_OFFSET	((unsigned long)0xffff800000000000)
#define	TASK_SIZE	((unsigned long)0x00007fffffffffff)

#define PAGE_GDT_SHIFT	39
#define PAGE_1G_SHIFT	30
#define PAGE_2M_SHIFT	21
#define PAGE_4K_SHIFT	12

#define PAGE_2M_SIZE	(1UL << PAGE_2M_SHIFT)
#define PAGE_4K_SIZE	(1UL << PAGE_4K_SHIFT)

#define PAGE_2M_MASK	(~ (PAGE_2M_SIZE - 1))
#define PAGE_4K_MASK	(~ (PAGE_4K_SIZE - 1))

#define PAGE_2M_ALIGN(addr)	(((unsigned long)(addr) + PAGE_2M_SIZE - 1) & PAGE_2M_MASK)
#define PAGE_4K_ALIGN(addr)	(((unsigned long)(addr) + PAGE_4K_SIZE - 1) & PAGE_4K_MASK)

#define Virt_To_Phy(addr)	((unsigned long)(addr) - PAGE_OFFSET)
#define Phy_To_Virt(addr)	((unsigned long *)((unsigned long)(addr) + PAGE_OFFSET))

#define Virt_To_2M_Page(kaddr)	(memory_management_struct.pages_struct + (Virt_To_Phy(kaddr) >> PAGE_2M_SHIFT))
#define Phy_to_2M_Page(kaddr)	(memory_management_struct.pages_struct + ((unsigned long)(kaddr) >> PAGE_2M_SHIFT))

////page table attribute

//	bit 63	Execution Disable:
#define PAGE_XD		(1UL << 63)

//	bit 12	Page Attribute Table
#define	PAGE_PAT	(1UL << 12)

//	bit 8	Global Page:1,global;0,part
#define	PAGE_Global	(1UL << 8)

//	bit 7	Page Size:1,big page;0,small page;
#define	PAGE_PS		(1UL << 7)

//	bit 6	Dirty:1,dirty;0,clean;
#define	PAGE_Dirty	(1UL << 6)

//	bit 5	Accessed:1,visited;0,unvisited;
#define	PAGE_Accessed	(1UL << 5)

//	bit 4	Page Level Cache Disable
#define PAGE_PCD	(1UL << 4)

//	bit 3	Page Level Write Through
#define PAGE_PWT	(1UL << 3)

//	bit 2	User Supervisor:1,user and supervisor;0,supervisor;
#define	PAGE_U_S	(1UL << 2)

//	bit 1	Read Write:1,read and write;0,read;
#define	PAGE_R_W	(1UL << 1)

//	bit 0	Present:1,present;0,no present;
#define	PAGE_Present	(1UL << 0)

//1,0
#define PAGE_KERNEL_GDT		(PAGE_R_W | PAGE_Present)

//1,0	
#define PAGE_KERNEL_Dir		(PAGE_R_W | PAGE_Present)

//7,1,0
#define	PAGE_KERNEL_Page	(PAGE_PS  | PAGE_R_W | PAGE_Present)

//1,0
#define PAGE_USER_GDT		(PAGE_U_S | PAGE_R_W | PAGE_Present)

//2,1,0
#define PAGE_USER_Dir		(PAGE_U_S | PAGE_R_W | PAGE_Present)

//7,2,1,0
#define	PAGE_USER_Page		(PAGE_PS  | PAGE_U_S | PAGE_R_W | PAGE_Present)

/*

*/

typedef struct {unsigned long pml4t;} pml4t_t;
#define	mk_mpl4t(addr,attr)	((unsigned long)(addr) | (unsigned long)(attr))
#define set_mpl4t(mpl4tptr,mpl4tval)	(*(mpl4tptr) = (mpl4tval))

typedef struct {unsigned long pdpt;} pdpt_t;
#define mk_pdpt(addr,attr)	((unsigned long)(addr) | (unsigned long)(attr))
#define set_pdpt(pdptptr,pdptval)	(*(pdptptr) = (pdptval))

typedef struct {unsigned long pdt;} pdt_t;
#define mk_pdt(addr,attr)	((unsigned long)(addr) | (unsigned long)(attr))
#define set_pdt(pdtptr,pdtval)		(*(pdtptr) = (pdtval))

typedef struct {unsigned long pt;} pt_t;
#define mk_pt(addr,attr)	((unsigned long)(addr) | (unsigned long)(attr))
#define set_pt(ptptr,ptval)		(*(ptptr) = (ptval))

extern unsigned long * Global_CR3;

struct E820
{
	unsigned long address;
	unsigned long length;
	unsigned int	type;
}__attribute__((packed));

/*

*/

struct Global_Memory_Descriptor
{
	struct E820 	e820[32];
	unsigned long 	e820_length;

	unsigned long * bits_map;
	unsigned long 	bits_size;
	unsigned long   bits_length;

	struct Page *	pages_struct;
	unsigned long	pages_size;
	unsigned long 	pages_length;

	struct Zone * 	zones_struct;
	unsigned long	zones_size;
	unsigned long 	zones_length;

	unsigned long 	start_code , end_code , end_data , end_brk;

	unsigned long	end_of_struct;	
};

////alloc_pages zone_select

//
#define ZONE_DMA	(1 << 0)

//
#define ZONE_NORMAL	(1 << 1)

//
#define ZONE_UNMAPED	(1 << 2)

////struct page attribute

//	mapped=1 or un-mapped=0 
#define PG_PTable_Maped	(1 << 0)

//	init-code=1 or normal-code/data=0
#define PG_Kernel_Init	(1 << 1)

//	device=1 or memory=0
#define PG_Device	(1 << 2)

//	kernel=1 or user=0
#define PG_Kernel	(1 << 3)

//	shared=1 or single-use=0 
#define PG_Shared	(1 << 4)


struct Page
{
	struct Zone *	zone_struct;
	unsigned long	PHY_address;
	unsigned long	attribute;

	unsigned long	reference_count;
	
	unsigned long	age;
};

//// each zone index

int ZONE_DMA_INDEX	= 0;
int ZONE_NORMAL_INDEX	= 0;	//low 1GB RAM ,was mapped in pagetable
int ZONE_UNMAPED_INDEX	= 0;	//above 1GB RAM,unmapped in pagetable

#define MAX_NR_ZONES	10	//max zone

/*

*/

struct Zone
{
	struct Page * 	pages_group;
	unsigned long	pages_length;
	
	unsigned long	zone_start_address;
	unsigned long	zone_end_address;
	unsigned long	zone_length;
	unsigned long	attribute;

	struct Global_Memory_Descriptor * GMD_struct;

	unsigned long	page_using_count;
	unsigned long	page_free_count;

	unsigned long	total_pages_link;
};

extern struct Global_Memory_Descriptor memory_management_struct;

/*

*/

struct Slab
{
	struct List list;
	struct Page * page;

	unsigned long using_count;
	unsigned long free_count;

	void * Vaddress;

	unsigned long color_length;
	unsigned long color_count;

	unsigned long * color_map;
};

struct Slab_cache
{
	unsigned long	size;
	unsigned long	total_using;
	unsigned long	total_free;
	struct Slab *	cache_pool;
	struct Slab *	cache_dma_pool;
	void *(* constructor)(void * Vaddress,unsigned long arg);
	void *(* destructor)(void * Vaddress,unsigned long arg);
};

/*
	kmalloc`s struct
*/

extern struct Slab_cache kmalloc_cache_size[16];

#define SIZEOF_LONG_ALIGN(size) ((size + sizeof(long) - 1) & ~(sizeof(long) - 1) )
#define SIZEOF_INT_ALIGN(size) ((size + sizeof(int) - 1) & ~(sizeof(int) - 1) )

/*

*/

#define	flush_tlb_one(addr)	\
	__asm__ __volatile__	("invlpg	(%0)	\n\t"::"r"(addr):"memory")
/*

*/

#define flush_tlb()						\
do								\
{								\
	unsigned long	tmpreg;					\
	__asm__ __volatile__ 	(				\
				"movq	%%cr3,	%0	\n\t"	\
				"movq	%0,	%%cr3	\n\t"	\
				:"=r"(tmpreg)			\
				:				\
				:"memory"			\
				);				\
}while(0)

/*

*/

inline unsigned long * Get_gdt()
{
	unsigned long * tmp;
	__asm__ __volatile__	(
					"movq	%%cr3,	%0	\n\t"
					:"=r"(tmp)
					:
					:"memory"
				);
	return tmp;
}

/*

*/

unsigned long page_init(struct Page * page,unsigned long flags);

unsigned long page_clean(struct Page * page);

/*

*/

unsigned long get_page_attribute(struct Page * page);

unsigned long set_page_attribute(struct Page * page,unsigned long flags);

/*

*/

void init_memory();

/*

*/

struct Page * alloc_pages(int zone_select,int number,unsigned long page_flags);

void free_pages(struct Page * page,int number);

/*
	return virtual kernel address
*/

void * kmalloc(unsigned long size,unsigned long flags);

/*

*/

struct Slab * kmalloc_create(unsigned long size);

/*

*/

unsigned long kfree(void * address);

/*

*/

struct Slab_cache * slab_create(unsigned long size,void *(* constructor)(void * Vaddress,unsigned long arg),void *(* destructor)(void * Vaddress,unsigned long arg),unsigned long arg);

/*

*/

unsigned long slab_destroy(struct Slab_cache * slab_cache);

/*

*/

void * slab_malloc(struct Slab_cache * slab_cache,unsigned long arg);

/*

*/

unsigned long slab_free(struct Slab_cache * slab_cache,void * address,unsigned long arg);

/*

*/

unsigned long slab_init();

/*

*/

void pagetable_init();

#endif
