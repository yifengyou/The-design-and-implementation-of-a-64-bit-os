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

#include "stddef.h"
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "string.h"
#include "keyboard.h"

int analysis_keycode(int fd);

int main()
{
	int fd0 = 0;
	int fd1 = 0;
	char path[] = "/KEYBOARD.DEV";
	int key = 0;

	fd1 = open(path,0);

	fd0 = 500;
	while(fd0--)
	{
		key = analysis_keycode(fd1);
		if(key)
			printf("(K:%c)",key);
	}

	close(fd1);

	while(1);
	return 0;
}

/*

*/

unsigned char get_scancode(int fd)
{
	unsigned char ret  = 0;
	read(fd,&ret,1);
	return ret;
}

/*

*/

int analysis_keycode(int fd)
{
	unsigned char x = 0;
	int i;	
	int key = 0;	
	int make = 0;

	x = get_scancode(fd);
	
	if(x == 0xE1)	//pause break;
	{
		key = PAUSEBREAK;
		for(i = 1;i<6;i++)
			if(get_scancode(fd) != pausebreak_scode[i])
			{
				key = 0;
				break;
			}
	}	
	else if(x == 0xE0) //print screen
	{
		x = get_scancode(fd);

		switch(x)
		{
			case 0x2A: // press printscreen
		
				if(get_scancode(fd) == 0xE0)
					if(get_scancode(fd) == 0x37)
					{
						key = PRINTSCREEN;
						make = 1;
					}
				break;

			case 0xB7: // UNpress printscreen
		
				if(get_scancode(fd) == 0xE0)
					if(get_scancode(fd) == 0xAA)
					{
						key = PRINTSCREEN;
						make = 0;
					}
				break;

			case 0x1d: // press right ctrl
		
				ctrl_r = 1;
				key = OTHERKEY;
				break;

			case 0x9d: // UNpress right ctrl
		
				ctrl_r = 0;
				key = OTHERKEY;
				break;
			
			case 0x38: // press right alt
		
				alt_r = 1;
				key = OTHERKEY;
				break;

			case 0xb8: // UNpress right alt
		
				alt_r = 0;
				key = OTHERKEY;
				break;		

			default:
				key = OTHERKEY;
				break;
		}
		
	}
	
	if(key == 0)
	{
		unsigned int * keyrow = NULL;
		int column = 0;

		make = (x & FLAG_BREAK ? 0:1);

		keyrow = &keycode_map_normal[(x & 0x7F) * MAP_COLS];

		if(shift_l || shift_r)
			column = 1;

		key = keyrow[column];
		
		switch(x & 0x7F)
		{
			case 0x2a:	//SHIFT_L:
				shift_l = make;
				key = 0;
				break;

			case 0x36:	//SHIFT_R:
				shift_r = make;
				key = 0;
				break;

			case 0x1d:	//CTRL_L:
				ctrl_l = make;
				key = 0;
				break;

			case 0x38:	//ALT_L:
				alt_l = make;
				key = 0;
				break;

			case 0x01:	//ESC
				key = 0;
				break;

			case 0x0e:	//BACKSPACE
				key = '\b';
				break;

			case 0x0f:	//TAB
				key = '\t';
				break;
	
			case 0x1c:	//ENTER
				key = '\n';
				break;

			default:
				if(!make)
					key = 0;
				break;
		}			

		if(key)
			return key;
	}
		return 0;
}



