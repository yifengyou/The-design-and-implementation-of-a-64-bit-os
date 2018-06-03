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

#ifndef __DIRENT_H__

#define __DIRENT_H__

struct dirent
{
	long d_offset;
	long d_type;
	long d_namelen;
	char d_name[];
};

struct DIR
{
	int fd;
	int buf_pos;
	int buf_end;
	char buf[256];
};

int getdents(int fd,struct dirent *buf,long count);

struct DIR* opendir(const char *path);
int closedir(struct DIR *dir);
struct dirent *readdir(struct DIR *dir);

#endif
