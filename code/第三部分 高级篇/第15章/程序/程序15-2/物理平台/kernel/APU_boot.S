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

#include "linkage.h"

.balign	 0x1000

.text
.code16

ENTRY(_APU_boot_start)

_APU_boot_base = .

	cli
	wbinvd

	mov	%cs,	%ax
	mov	%ax,	%ds
	mov	%ax,	%es
	mov	%ax,	%ss
	mov	%ax,	%fs
	mov	%ax,	%gs

#	set sp

	movl	$(_APU_boot_tmp_Stack_end - _APU_boot_base),	%esp

#	get base address

	mov	%cs,	%ax
	movzx	%ax,	%esi
	shll	$4,	%esi

#	set gdt and 32&64 code address

	leal	(_APU_Code32 - _APU_boot_base)(%esi),	%eax
	movl	%eax,	_APU_Code32_vector - _APU_boot_base

	leal	(_APU_Code64 - _APU_boot_base)(%esi),	%eax
	movl	%eax,	_APU_Code64_vector - _APU_boot_base

	leal	(_APU_tmp_GDT - _APU_boot_base)(%esi),	%eax
	movl	%eax,	(_APU_tmp_GDT + 2 - _APU_boot_base)
	
#	load idt gdt
	
	lidtl	_APU_tmp_IDT - _APU_boot_base
	lgdtl	_APU_tmp_GDT - _APU_boot_base

#	enable protected mode

	smsw	%ax
	bts	$0	,%ax
	lmsw	%ax

#	go to 32 code
	ljmpl	*(_APU_Code32_vector - _APU_boot_base)

.code32
.balign 4
_APU_Code32:
#	go to 64 code
	
	mov	$0x10,	%ax
	mov	%ax,	%ds
	mov	%ax,	%es
	mov	%ax,	%ss
	mov	%ax,	%fs
	mov	%ax,	%gs

	leal	(_APU_boot_tmp_Stack_end - _APU_boot_base)(%esi),	%eax
	movl	%eax,	%esp
	

#	open PAE

	movl	%cr4,	%eax
	bts	$5,	%eax
	movl	%eax,	%cr4

#	set page table

	movl	$0x90000,	%eax
	movl	%eax,	%cr3

#	enable long mode

	movl	$0xC0000080,	%ecx
	rdmsr

	bts	$8,	%eax
	wrmsr

#	enable PE & paging

	movl	%cr0,	%eax
	bts	$0,	%eax
	bts	$31,	%eax
	movl	%eax,	%cr0
	
	ljmp	*(_APU_Code64_vector - _APU_boot_base)(%esi)


.code64
.balign 4
_APU_Code64:
#	go to head.S
	movq	$0x20,	%rax
	movq	%rax,	%ds
	movq	%rax,	%es
	movq	%rax,	%fs
	movq	%rax,	%gs
	movq	%rax,	%ss

	movq	$0x100000,	%rax
	jmpq	*%rax

	hlt

.balign 4
_APU_tmp_IDT:
	.word	0
	.word	0,0

.balign 4
_APU_tmp_GDT:
	.short	_APU_tmp_GDT_end - _APU_tmp_GDT - 1
	.long	_APU_tmp_GDT - _APU_boot_base
	.short	0
	.quad	0x00cf9a000000ffff
	.quad	0x00cf92000000ffff
	.quad	0x0020980000000000
	.quad	0x0000920000000000
_APU_tmp_GDT_end:

.balign 4
_APU_Code32_vector:
	.long	_APU_Code32 - _APU_boot_base
	.word	0x08,0	

.balign 4
_APU_Code64_vector:
	.long	_APU_Code64 - _APU_boot_base
	.word	0x18,0	

.balign 4
_APU_boot_tmp_Stack_start:
	.org	0x400
_APU_boot_tmp_Stack_end:

ENTRY(_APU_boot_end)
