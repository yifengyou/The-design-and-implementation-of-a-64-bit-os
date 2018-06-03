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


#include "errno.h"
#include "printk.h"
#include "lib.h"
#include "VFS.h"
#include "fcntl.h"
#include "memory.h"
#include "fat32.h"
#include "stdio.h"
#include "fcntl.h"

/*
normal
system call number:	rax
arg1:	rdi
arg2:	rsi
arg3:	rdx
arg4:	rcx
arg5:	r8
arg6:	r9

sysenter need rdx(rip) rcx(rsp)
syscall need rcx(rip) r11(rflags)

xchg rdx to r10, rcx to r11

*/

unsigned long no_system_call(void)
{
	color_printk(RED,BLACK,"no_system_call is calling\n");
	return -ENOSYS;
}

/*	
__asm__	(
".global puts	\n\t"
".type	puts,	@function \n\t"
"puts:		\n\t"
"pushq	%r10	\n\t"
"pushq	%r11	\n\t"
"movq	$__NR_puts	,	%rax	\n\t"
"leaq	sysexit_return_address(%rip),	%r10	\n\t"
"movq	%rsp,	%r11		\n\t"
"sysenter			\n\t"
"sysexit_return_address:	\n\t"
"xchgq	%rdx,	%r10	\n\t"
"xchgq	%rcx,	%r11	\n\t"
"popq	%r11	\n\t"
"popq	%r10	\n\t"
);	
*/


unsigned long sys_putstring(char *string)
{
	color_printk(GREEN,BLACK,"sys_putstring\n");
	color_printk(ORANGE,WHITE,string);
	return 0;
}

unsigned long sys_open(char *filename,int flags)
{
	char * path = NULL;
	long pathlen = 0;
	long error = 0;
	struct dir_entry * dentry = NULL;
	struct file * filp = NULL;
	struct file ** f = NULL;
	int fd = -1;
	int i;

	color_printk(GREEN,BLACK,"sys_open\n");
	path = (char *)kmalloc(PAGE_4K_SIZE,0);
	if(path == NULL)
		return -ENOMEM;
	memset(path,0,PAGE_4K_SIZE);
	pathlen = strnlen_user(filename,PAGE_4K_SIZE);
	if(pathlen <= 0)
	{
		kfree(path);
		return -EFAULT;
	}	
	else if(pathlen >= PAGE_4K_SIZE)
	{
		kfree(path);
		return -ENAMETOOLONG;
	}
	strncpy_from_user(filename,path,pathlen);

	dentry = path_walk(path,0);
	kfree(path);

/////////////////
	if(dentry != NULL)
		color_printk(BLUE,WHITE,"Find 89AIOlejk.TXT\nDIR_FirstCluster:%#018lx\tDIR_FileSize:%#018lx\n",((struct FAT32_inode_info *)(dentry->dir_inode->private_index_info))->first_cluster,dentry->dir_inode->file_size);
	else
		color_printk(BLUE,WHITE,"Can`t find file\n");

	if(dentry == NULL)
		return -ENOENT;
	if(dentry->dir_inode->attribute == FS_ATTR_DIR)
		return -EISDIR;

	filp = (struct file *)kmalloc(sizeof(struct file),0);
	memset(filp,0,sizeof(struct file));
	filp->dentry = dentry;
	filp->mode = flags;
	filp->f_ops = dentry->dir_inode->f_ops;
	if(filp->f_ops && filp->f_ops->open)
		error = filp->f_ops->open(dentry->dir_inode,filp);
	if(error != 1)
	{
		kfree(filp);
		return -EFAULT;
	}

	if(filp->mode & O_TRUNC)
	{
		filp->dentry->dir_inode->file_size = 0;
	}
	if(filp->mode & O_APPEND)
	{
		filp->position = filp->dentry->dir_inode->file_size;
	}

	f = current->file_struct;
	for(i = 0;i < TASK_FILE_MAX;i++)
		if(f[i] == NULL)
		{
			fd = i;
			break;
		}
	if(i == TASK_FILE_MAX)
	{
		kfree(filp);
		//// reclaim struct index_node & struct dir_entry
		return -EMFILE;
	}
	f[fd] = filp;

	return fd;
}

unsigned long sys_close(int fd)
{
	struct file * filp = NULL;

	color_printk(GREEN,BLACK,"sys_close:%d\n",fd);
	if(fd < 0 || fd >= TASK_FILE_MAX)
		return -EBADF;

	filp = current->file_struct[fd];
	if(filp->f_ops && filp->f_ops->close)
		filp->f_ops->close(filp->dentry->dir_inode,filp);

	kfree(filp);
	current->file_struct[fd] = NULL;

	return 0;
}
