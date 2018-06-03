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

#include "dirent.h"
#include "unistd.h"
#include "fcntl.h"
#include "stddef.h"
#include "stdlib.h"

struct DIR* opendir(const char *path)
{
	int fd = 0;
	struct DIR* dir = NULL;

	fd = open(path,O_DIRECTORY);

	if(fd >= 0)
		dir = (struct DIR*)malloc(sizeof(struct DIR));
	else
		return NULL;
	memset(dir,0,sizeof(struct DIR));
	dir->fd = fd;
	dir->buf_pos = 0;
	dir->buf_end = 256;
	
	return dir;
}

int closedir(struct DIR *dir)
{
	close(dir->fd);
	free(dir);
	return 0;
}

struct dirent *readdir(struct DIR *dir)
{
	int len = 0;
	memset(dir->buf,0,256);
	len = getdents(dir->fd,(struct dirent *)dir->buf,256);
	if(len > 0)
		return (struct dirent *)dir->buf;
	else
		return NULL;
}

