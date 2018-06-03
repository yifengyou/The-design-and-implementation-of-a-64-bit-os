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
#include "ptrace.h"
#include "task.h"
#include "sched.h"
#include "memory.h"
#include "keyboard.h"

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

unsigned long sys_putstring(char *string)
{
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

	if(dentry == NULL)
		return -ENOENT;
	if(dentry->dir_inode->attribute == FS_ATTR_DIR)
		return -EISDIR;

	filp = (struct file *)kmalloc(sizeof(struct file),0);
	memset(filp,0,sizeof(struct file));
	filp->dentry = dentry;
	filp->mode = flags;

	if(dentry->dir_inode->attribute & FS_ATTR_DEVICE)
		filp->f_ops = &keyboard_fops;	//////	find device file operation function
	else
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

unsigned long sys_read(int fd,void * buf,long count)
{
	struct file * filp = NULL;
	unsigned long ret = 0;

	color_printk(GREEN,BLACK,"sys_read:%d\n",fd);
	if(fd < 0 || fd >= TASK_FILE_MAX)
		return -EBADF;
	if(count < 0)
		return -EINVAL;

	filp = current->file_struct[fd];
	if(filp->f_ops && filp->f_ops->read)
		ret = filp->f_ops->read(filp,buf,count,&filp->position);
	return ret;
}

unsigned long sys_write(int fd,void * buf,long count)
{
	struct file * filp = NULL;
	unsigned long ret = 0;

	color_printk(GREEN,BLACK,"sys_write:%d\n",fd);
	if(fd < 0 || fd >= TASK_FILE_MAX)
		return -EBADF;
	if(count < 0)
		return -EINVAL;

	filp = current->file_struct[fd];
	if(filp->f_ops && filp->f_ops->write)
		ret = filp->f_ops->write(filp,buf,count,&filp->position);
	return ret;
}


unsigned long sys_lseek(int filds,long offset,int whence)
{
	struct file * filp = NULL;
	unsigned long ret = 0;

	color_printk(GREEN,BLACK,"sys_lseek:%d\n",filds);
	if(filds < 0 || filds >= TASK_FILE_MAX)
		return -EBADF;
	if(whence < 0 || whence >= SEEK_MAX)
		return -EINVAL;

	filp = current->file_struct[filds];
	if(filp->f_ops && filp->f_ops->lseek)
		ret = filp->f_ops->lseek(filp,offset,whence);
	return ret;
}

unsigned long sys_fork()
{
	struct pt_regs *regs = (struct pt_regs *)current->thread->rsp0 -1;
	color_printk(GREEN,BLACK,"sys_fork\n");
	return do_fork(regs,0,regs->rsp,0);	
}

unsigned long sys_vfork()
{
	struct pt_regs *regs = (struct pt_regs *)current->thread->rsp0 -1;
	color_printk(GREEN,BLACK,"sys_vfork\n");
	return do_fork(regs,CLONE_VM | CLONE_FS | CLONE_SIGNAL,regs->rsp,0);
}

unsigned long sys_brk(unsigned long brk)
{
	unsigned long new_brk = PAGE_2M_ALIGN(brk);

	color_printk(GREEN,BLACK,"sys_brk\n");
//	color_printk(RED,BLACK,"brk:%#018lx,new_brk:%#018lx,current->mm->end_brk:%#018lx\n",brk,new_brk,current->mm->end_brk);
	if(new_brk == 0)
		return current->mm->start_brk;
	if(new_brk < current->mm->end_brk)	//release  brk space
		return 0;	

	new_brk = do_brk(current->mm->end_brk,new_brk - current->mm->end_brk);	//expand brk space

	current->mm->end_brk = new_brk;
	return new_brk;
}


