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
#ifndef	__MOUSE_H__

#define	__MOUSE_H__

extern struct keyboard_inputbuffer * p_mouse;

#define KBCMD_SENDTO_MOUSE	0xd4
#define MOUSE_ENABLE		0xf4

#define KBCMD_EN_MOUSE_INTFACE	0xa8

struct mouse_packet
{	
	unsigned char Byte0;	//7:Y overflow,6:X overflow,5:Y sign bit,4:X sign bit,3:Always,2:Middle Btn,1:Right Btn,0:Left Btn
	char Byte1;	//X movement
	char Byte2;	//Y movement
};

extern struct mouse_packet mouse;

/*

*/

void mouse_init();
void mouse_exit();

/*

*/

void analysis_mousecode();

#endif
