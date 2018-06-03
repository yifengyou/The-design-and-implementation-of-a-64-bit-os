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

#ifndef __SYSCALL_H__

#define __SYSCALL_H__

#define	__NR_putstring	1

#define	__NR_open	2
#define	__NR_close	3
#define	__NR_read	4
#define	__NR_write	5
#define	__NR_lseek	6

#define	__NR_fork	7
#define	__NR_vfork	8
#define	__NR_execve	9
#define	__NR_exit	10
#define	__NR_wait4	11

#define __NR_brk	12
#define __NR_reboot	13

#define __NR_chdir	14
#define __NR_getdents	15

#endif
