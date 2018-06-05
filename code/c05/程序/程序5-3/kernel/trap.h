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

#ifndef __TRAP_H__

#define __TRAP_H__

#include "linkage.h"
#include "printk.h"
#include "lib.h"

/*

*/

 void divide_error();
 void debug();
 void nmi();
 void int3();
 void overflow();
 void bounds();
 void undefined_opcode();
 void dev_not_available();
 void double_fault();
 void coprocessor_segment_overrun();
 void invalid_TSS();
 void segment_not_present();
 void stack_segment_fault();
 void general_protection();
 void page_fault();
 void x87_FPU_error();
 void alignment_check();
 void machine_check();
 void SIMD_exception();
 void virtualization_exception();



/*

*/

void sys_vector_init();


#endif
