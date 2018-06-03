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

#include "stdio.h"
#include "stdlib.h"

int main(int argc,char *argv[])
{
	int i = 0;
	printf("Hello World!\n");
	printf("argc:%d,argv:%#018lx\n",argc,argv);
	for(i = 0;i<argc;i++)
		printf("argv[%d]:%s\n",i,argv[i]);
	exit(0);
	return 0;
}

