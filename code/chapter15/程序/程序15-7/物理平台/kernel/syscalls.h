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


SYSCALL_COMMON(__NR_putstring,sys_putstring)

SYSCALL_COMMON(__NR_open,sys_open)
SYSCALL_COMMON(__NR_close,sys_close)
SYSCALL_COMMON(__NR_read,sys_read)
SYSCALL_COMMON(__NR_write,sys_write)
SYSCALL_COMMON(__NR_lseek,sys_lseek)

SYSCALL_COMMON(__NR_fork,sys_fork)
SYSCALL_COMMON(__NR_vfork,sys_vfork)
SYSCALL_COMMON(__NR_execve,sys_execve)
SYSCALL_COMMON(__NR_exit,sys_exit)
SYSCALL_COMMON(__NR_wait4,sys_wait4)

SYSCALL_COMMON(__NR_brk,sys_brk)
SYSCALL_COMMON(__NR_reboot,sys_reboot)

SYSCALL_COMMON(__NR_chdir,sys_chdir)
SYSCALL_COMMON(__NR_getdents,sys_getdents)

