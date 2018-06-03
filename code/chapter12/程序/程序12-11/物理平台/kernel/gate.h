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

#ifndef __GATE_H__
#define __GATE_H__

struct desc_struct 
{
	unsigned char x[8];
};

struct gate_struct
{
	unsigned char x[16];
};

extern struct desc_struct GDT_Table[];
extern struct gate_struct IDT_Table[];
extern unsigned int TSS64_Table[26];

/*

*/

#define _set_gate(gate_selector_addr,attr,ist,code_addr)	\
do								\
{	unsigned long __d0,__d1;				\
	__asm__ __volatile__	(	"movw	%%dx,	%%ax	\n\t"	\
					"andq	$0x7,	%%rcx	\n\t"	\
					"addq	%4,	%%rcx	\n\t"	\
					"shlq	$32,	%%rcx	\n\t"	\
					"addq	%%rcx,	%%rax	\n\t"	\
					"xorq	%%rcx,	%%rcx	\n\t"	\
					"movl	%%edx,	%%ecx	\n\t"	\
					"shrq	$16,	%%rcx	\n\t"	\
					"shlq	$48,	%%rcx	\n\t"	\
					"addq	%%rcx,	%%rax	\n\t"	\
					"movq	%%rax,	%0	\n\t"	\
					"shrq	$32,	%%rdx	\n\t"	\
					"movq	%%rdx,	%1	\n\t"	\
					:"=m"(*((unsigned long *)(gate_selector_addr)))	,					\
					 "=m"(*(1 + (unsigned long *)(gate_selector_addr))),"=&a"(__d0),"=&d"(__d1)		\
					:"i"(attr << 8),									\
					 "3"((unsigned long *)(code_addr)),"2"(0x8 << 16),"c"(ist)				\
					:"memory"		\
				);				\
}while(0)

/*

*/

inline void set_tss_descriptor(unsigned int n,void * addr)
{
	unsigned long limit = 103;
	*(unsigned long *)(GDT_Table + n) = (limit & 0xffff) | (((unsigned long)addr & 0xffff) << 16) | (((unsigned long)addr >> 16 & 0xff) << 32) | ((unsigned long)0x89 << 40) | ((limit >> 16 & 0xf) << 48) | (((unsigned long)addr >> 24 & 0xff) << 56);	/////89 is attribute
	*(unsigned long *)(GDT_Table + n + 1) = ((unsigned long)addr >> 32 & 0xffffffff) | 0;
}

/*

*/

#define load_TR(n) 							\
do{									\
	__asm__ __volatile__(	"ltr	%%ax"				\
				:					\
				:"a"(n << 3)				\
				:"memory");				\
}while(0)

/*

*/

inline void set_intr_gate(unsigned int n,unsigned char ist,void * addr)
{
	_set_gate(IDT_Table + n , 0x8E , ist , addr);	//P,DPL=0,TYPE=E
}

/*

*/

inline void set_trap_gate(unsigned int n,unsigned char ist,void * addr)
{
	_set_gate(IDT_Table + n , 0x8F , ist , addr);	//P,DPL=0,TYPE=F
}

/*

*/

inline void set_system_gate(unsigned int n,unsigned char ist,void * addr)
{
	_set_gate(IDT_Table + n , 0xEF , ist , addr);	//P,DPL=3,TYPE=F
}

/*

*/

inline void set_system_intr_gate(unsigned int n,unsigned char ist,void * addr)	//int3
{
	_set_gate(IDT_Table + n , 0xEE , ist , addr);	//P,DPL=3,TYPE=E
}


/*

*/

void set_tss64(unsigned int * Table,unsigned long rsp0,unsigned long rsp1,unsigned long rsp2,unsigned long ist1,unsigned long ist2,unsigned long ist3,
unsigned long ist4,unsigned long ist5,unsigned long ist6,unsigned long ist7)
{
	*(unsigned long *)(Table+1) = rsp0;
	*(unsigned long *)(Table+3) = rsp1;
	*(unsigned long *)(Table+5) = rsp2;

	*(unsigned long *)(Table+9) = ist1;
	*(unsigned long *)(Table+11) = ist2;
	*(unsigned long *)(Table+13) = ist3;
	*(unsigned long *)(Table+15) = ist4;
	*(unsigned long *)(Table+17) = ist5;
	*(unsigned long *)(Table+19) = ist6;
	*(unsigned long *)(Table+21) = ist7;	
}


#endif

