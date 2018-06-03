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

#ifndef __KEYBOARD_H__

#define __KEYBOARD_H__


#define NR_SCAN_CODES 	0x80
#define MAP_COLS	2


#define PAUSEBREAK	1
#define PRINTSCREEN	2
#define OTHERKEY	4
#define FLAG_BREAK	0x80

/*

*/

extern int shift_l,shift_r,ctrl_l,ctrl_r,alt_l,alt_r;

extern unsigned char pausebreak_scode[];

extern unsigned int keycode_map_normal[NR_SCAN_CODES * MAP_COLS];

#endif
