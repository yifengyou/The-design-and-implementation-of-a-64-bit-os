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
#include "printk.h"
#include "dirent.h"
#include "errno.h"

struct dir_entry * path_walk(char * name,unsigned long flags)
{
	char * tmpname = NULL;
	int tmpnamelen = 0;
	struct dir_entry * parent = root_sb->root;
	struct dir_entry * path = NULL;

	while(*name == '/')
		name++;

	if(!*name)
	{
		return parent;
	}

	for(;;)
	{
		tmpname = name;
		while(*name && (*name != '/'))
			name++;
		tmpnamelen = name - tmpname;

		path = (struct dir_entry *)kmalloc(sizeof(struct dir_entry),0);
		memset(path,0,sizeof(struct dir_entry));

		path->name = kmalloc(tmpnamelen+1,0);
		memset(path->name,0,tmpnamelen+1);
		memcpy(tmpname,path->name,tmpnamelen);
		path->name_length = tmpnamelen;

		if(parent->dir_inode->inode_ops->lookup(parent->dir_inode,path) == NULL)
		{
			color_printk(RED,WHITE,"can not find file or dir:%s\n",path->name);
			kfree(path->name);
			kfree(path);
			return NULL;
		}

		list_init(&path->child_node);
		list_init(&path->subdirs_list);
		path->parent = parent;
		list_add_to_behind(&parent->subdirs_list,&path->child_node);

		if(!*name)
			goto last_component;
		while(*name == '/')
			name++;
		if(!*name)
			goto last_slash;

		parent = path;
	}

last_slash:
last_component:

	if(flags & 1)
	{
		return parent;
	}

	return path;
}

int fill_dentry(void *buf,char *name, long namelen,long type,long offset)
{
	struct dirent* dent = (struct dirent*)buf;
	
	if((unsigned long)buf < TASK_SIZE && !verify_area(buf,sizeof(struct dirent) + namelen))
		return -EFAULT;
	
	memcpy(name,dent->d_name,namelen);
	dent->d_namelen = namelen;
	dent->d_type = type;
	dent->d_offset = offset;
	return sizeof(struct dirent) + namelen;
}

//function mount_root
struct super_block * root_sb = NULL;
struct file_system_type filesystem = {"filesystem",0};

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


