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

#include "VFS.h"
#include "lib.h"


struct file_system_type filesystem = {"filesystem",0};

//function mount_root

struct super_block* mount_fs(char * name,struct Disk_Partition_Table_Entry * DPTE,void * buf)
{
	struct file_system_type * p = NULL;

	for(p = &filesystem;p;p = p->next)
		if(!strcmp(p->name,name))
		{
			return p->read_superblock(DPTE,buf);
		}
	return 0;
}

unsigned long register_filesystem(struct file_system_type * fs)
{
	struct file_system_type * p = NULL;

	for(p = &filesystem;p;p = p->next)
		if(!strcmp(fs->name,p->name))
			return 0;

	fs->next = filesystem.next;
	filesystem.next = fs;

	return 1;
}

unsigned long unregister_filesystem(struct file_system_type * fs)
{
	struct file_system_type * p = &filesystem;

	while(p->next)
		if(p->next == fs)
		{
			p->next = p->next->next;
			fs->next = NULL;
			return 1;
		}
		else
			p = p->next;
	return 0;
}


