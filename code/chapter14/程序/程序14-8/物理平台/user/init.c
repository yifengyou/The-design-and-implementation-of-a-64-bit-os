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
#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"

int main()
{
	int fd = 0;
	char string[]="/JIOL123Llliwos/89AIOlejk.TXT";
	unsigned char buf[32] = {0};

	fd = open(string,0);
	write(fd,string,20);
	lseek(fd,5,SEEK_SET);
	read(fd,buf,30);
	close(fd);
	putstring(buf);

	if(fork() == 0)
		putstring("child process\n");
	else
	{
		putstring("parent process\n");
		malloc(100);
	}	

	while(1);
	return 0;
}



