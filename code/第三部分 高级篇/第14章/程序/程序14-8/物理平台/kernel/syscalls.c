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

#include "unistd.h"

#define SYSCALL_COMMON(nr,sym)	extern unsigned long sym(void);
SYSCALL_COMMON(0,no_system_call)
#include "syscalls.h"
#undef	SYSCALL_COMMON


#define SYSCALL_COMMON(nr,sym)	[nr] = sym,


#define MAX_SYSTEM_CALL_NR 128
typedef unsigned long (* system_call_t)(void);


system_call_t system_call_table[MAX_SYSTEM_CALL_NR] = 
{
	[0 ... MAX_SYSTEM_CALL_NR-1] = no_system_call,
#include "syscalls.h"
};

