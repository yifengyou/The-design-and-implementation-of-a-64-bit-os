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

#ifndef __SMP_H__

#define __SMP_H__

#include "spinlock.h"

extern unsigned char _APU_boot_start[];
extern unsigned char _APU_boot_end[];

extern spinlock_T SMP_lock;

extern int global_i;

#define SMP_cpu_id()	(current->cpu_id)

void SMP_init();

void Start_SMP();

#endif
