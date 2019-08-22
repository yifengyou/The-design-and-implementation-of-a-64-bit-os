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

#ifndef __LIB_H__
#define __LIB_H__


#define NULL 0

#define container_of(ptr,type,member)							\
({											\
	typeof(((type *)0)->member) * p = (ptr);					\
	(type *)((unsigned long)p - (unsigned long)&(((type *)0)->member));		\
})


#define sti() 		__asm__ __volatile__ ("sti	\n\t":::"memory")
#define cli()	 	__asm__ __volatile__ ("cli	\n\t":::"memory")
#define nop() 		__asm__ __volatile__ ("nop	\n\t")
#define io_mfence() 	__asm__ __volatile__ ("mfence	\n\t":::"memory")


struct List
{
	struct List * prev;
	struct List * next;
};

inline void list_init(struct List * list)
{
	list->prev = list;
	list->next = list;
}

inline void list_add_to_behind(struct List * entry,struct List * new)	////add to entry behind
{
	new->next = entry->next;
	new->prev = entry;
	new->next->prev = new;
	entry->next = new;
}

inline void list_add_to_before(struct List * entry,struct List * new)	////add to entry behind
{
	new->next = entry;
	entry->prev->next = new;
	new->prev = entry->prev;
	entry->prev = new;
}

inline void list_del(struct List * entry)
{
	entry->next->prev = entry->prev;
	entry->prev->next = entry->next;
}

inline long list_is_empty(struct List * entry)
{
	if(entry == entry->next && entry->prev == entry)
		return 1;
	else
		return 0;
}

inline struct List * list_prev(struct List * entry)
{
	if(entry->prev != NULL)
		return entry->prev;
	else
		return NULL;
}

inline struct List * list_next(struct List * entry)
{
	if(entry->next != NULL)
		return entry->next;
	else
		return NULL;
}

/*
		From => To memory copy Num bytes
*/

inline void * memcpy(void *From,void * To,long Num)
{
	int d0,d1,d2;
	__asm__ __volatile__	(	"cld	\n\t"
					"rep	\n\t"
					"movsq	\n\t"
					"testb	$4,%b4	\n\t"
					"je	1f	\n\t"
					"movsl	\n\t"
					"1:\ttestb	$2,%b4	\n\t"
					"je	2f	\n\t"
					"movsw	\n\t"
					"2:\ttestb	$1,%b4	\n\t"
					"je	3f	\n\t"
					"movsb	\n\t"
					"3:	\n\t"
					:"=&c"(d0),"=&D"(d1),"=&S"(d2)
					:"0"(Num/8),"q"(Num),"1"(To),"2"(From)
					:"memory"
				);
	return To;
}

/*
		FirstPart = SecondPart		=>	 0
		FirstPart > SecondPart		=>	 1
		FirstPart < SecondPart		=>	-1
*/

inline int memcmp(void * FirstPart,void * SecondPart,long Count)
{
	register int __res;

	__asm__	__volatile__	(	"cld	\n\t"		//clean direct
					"repe	\n\t"		//repeat if equal
					"cmpsb	\n\t"
					"je	1f	\n\t"
					"movl	$1,	%%eax	\n\t"
					"jl	1f	\n\t"
					"negl	%%eax	\n\t"
					"1:	\n\t"
					:"=a"(__res)
					:"0"(0),"D"(FirstPart),"S"(SecondPart),"c"(Count)
					:
				);
	return __res;
}

/*
		set memory at Address with C ,number is Count
*/

inline void * memset(void * Address,unsigned char C,long Count)
{
	int d0,d1;
	unsigned long tmp = C * 0x0101010101010101UL;
	__asm__	__volatile__	(	"cld	\n\t"
					"rep	\n\t"
					"stosq	\n\t"
					"testb	$4, %b3	\n\t"
					"je	1f	\n\t"
					"stosl	\n\t"
					"1:\ttestb	$2, %b3	\n\t"
					"je	2f\n\t"
					"stosw	\n\t"
					"2:\ttestb	$1, %b3	\n\t"
					"je	3f	\n\t"
					"stosb	\n\t"
					"3:	\n\t"
					:"=&c"(d0),"=&D"(d1)
					:"a"(tmp),"q"(Count),"0"(Count/8),"1"(Address)	
					:"memory"					
				);
	return Address;
}

/*
		string copy
*/

inline char * strcpy(char * Dest,char * Src)
{
	__asm__	__volatile__	(	"cld	\n\t"
					"1:	\n\t"
					"lodsb	\n\t"
					"stosb	\n\t"
					"testb	%%al,	%%al	\n\t"
					"jne	1b	\n\t"
					:
					:"S"(Src),"D"(Dest)
					:
					
				);
	return 	Dest;
}

/*
		string copy number bytes
*/

inline char * strncpy(char * Dest,char * Src,long Count)
{
	__asm__	__volatile__	(	"cld	\n\t"
					"1:	\n\t"
					"decq	%2	\n\t"
					"js	2f	\n\t"
					"lodsb	\n\t"
					"stosb	\n\t"
					"testb	%%al,	%%al	\n\t"
					"jne	1b	\n\t"
					"rep	\n\t"
					"stosb	\n\t"
					"2:	\n\t"
					:
					:"S"(Src),"D"(Dest),"c"(Count)
					:					
				);
	return Dest;
}

/*
		string cat Dest + Src
*/

inline char * strcat(char * Dest,char * Src)
{
	__asm__	__volatile__	(	"cld	\n\t"
					"repne	\n\t"
					"scasb	\n\t"
					"decq	%1	\n\t"
					"1:	\n\t"
					"lodsb	\n\t"
					"stosb	\n\r"
					"testb	%%al,	%%al	\n\t"
					"jne	1b	\n\t"
					:
					:"S"(Src),"D"(Dest),"a"(0),"c"(0xffffffff)
					:					
				);
	return Dest;
}

/*
		string compare FirstPart and SecondPart
		FirstPart = SecondPart =>  0
		FirstPart > SecondPart =>  1
		FirstPart < SecondPart => -1
*/

inline int strcmp(char * FirstPart,char * SecondPart)
{
	register int __res;
	__asm__	__volatile__	(	"cld	\n\t"
					"1:	\n\t"
					"lodsb	\n\t"
					"scasb	\n\t"
					"jne	2f	\n\t"
					"testb	%%al,	%%al	\n\t"
					"jne	1b	\n\t"
					"xorl	%%eax,	%%eax	\n\t"
					"jmp	3f	\n\t"
					"2:	\n\t"
					"movl	$1,	%%eax	\n\t"
					"jl	3f	\n\t"
					"negl	%%eax	\n\t"
					"3:	\n\t"
					:"=a"(__res)
					:"D"(FirstPart),"S"(SecondPart)
					:					
				);
	return __res;
}

/*
		string compare FirstPart and SecondPart with Count Bytes
		FirstPart = SecondPart =>  0
		FirstPart > SecondPart =>  1
		FirstPart < SecondPart => -1
*/

inline int strncmp(char * FirstPart,char * SecondPart,long Count)
{	
	register int __res;
	__asm__	__volatile__	(	"cld	\n\t"
					"1:	\n\t"
					"decq	%3	\n\t"
					"js	2f	\n\t"
					"lodsb	\n\t"
					"scasb	\n\t"
					"jne	3f	\n\t"
					"testb	%%al,	%%al	\n\t"
					"jne	1b	\n\t"
					"2:	\n\t"
					"xorl	%%eax,	%%eax	\n\t"
					"jmp	4f	\n\t"
					"3:	\n\t"
					"movl	$1,	%%eax	\n\t"
					"jl	4f	\n\t"
					"negl	%%eax	\n\t"
					"4:	\n\t"
					:"=a"(__res)
					:"D"(FirstPart),"S"(SecondPart),"c"(Count)
					:
				);
	return __res;
}

/*

*/

inline int strlen(char * String)
{
	register int __res;
	__asm__	__volatile__	(	"cld	\n\t"
					"repne	\n\t"
					"scasb	\n\t"
					"notl	%0	\n\t"
					"decl	%0	\n\t"
					:"=c"(__res)
					:"D"(String),"a"(0),"0"(0xffffffff)
					:
				);
	return __res;
}

/*

*/

inline unsigned long bit_set(unsigned long * addr,unsigned long nr)
{
	return *addr | (1UL << nr);
}

/*

*/

inline unsigned long bit_get(unsigned long * addr,unsigned long nr)
{
	return	*addr & (1UL << nr);
}

/*

*/

inline unsigned long bit_clean(unsigned long * addr,unsigned long nr)
{
	return	*addr & (~(1UL << nr));
}

/*

*/

inline unsigned char io_in8(unsigned short port)
{
	unsigned char ret = 0;
	__asm__ __volatile__(	"inb	%%dx,	%0	\n\t"
				"mfence			\n\t"
				:"=a"(ret)
				:"d"(port)
				:"memory");
	return ret;
}

/*

*/

inline unsigned int io_in32(unsigned short port)
{
	unsigned int ret = 0;
	__asm__ __volatile__(	"inl	%%dx,	%0	\n\t"
				"mfence			\n\t"
				:"=a"(ret)
				:"d"(port)
				:"memory");
	return ret;
}

/*

*/

inline void io_out8(unsigned short port,unsigned char value)
{
	__asm__ __volatile__(	"outb	%0,	%%dx	\n\t"
				"mfence			\n\t"
				:
				:"a"(value),"d"(port)
				:"memory");
}

/*

*/

inline void io_out32(unsigned short port,unsigned int value)
{
	__asm__ __volatile__(	"outl	%0,	%%dx	\n\t"
				"mfence			\n\t"
				:
				:"a"(value),"d"(port)
				:"memory");
}

/*

*/

#define port_insw(port,buffer,nr)	\
__asm__ __volatile__("cld;rep;insw;mfence;"::"d"(port),"D"(buffer),"c"(nr):"memory")

#define port_outsw(port,buffer,nr)	\
__asm__ __volatile__("cld;rep;outsw;mfence;"::"d"(port),"S"(buffer),"c"(nr):"memory")

#endif
