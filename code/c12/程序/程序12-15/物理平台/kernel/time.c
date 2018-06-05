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

#include "time.h"
#include "lib.h"

#define CMOS_READ(addr) ({ \
io_out8(0x70,0x80 | addr); \
io_in8(0x71); \
})

int get_cmos_time(struct time *time)
{
	do
	{	time->year =   CMOS_READ(0x09) + CMOS_READ(0x32) * 0x100;
		time->month =  CMOS_READ(0x08);
		time->day =    CMOS_READ(0x07);	
		time->hour =   CMOS_READ(0x04);	
		time->minute = CMOS_READ(0x02);
		time->second = CMOS_READ(0x00);
	}while(time->second != CMOS_READ(0x00));
	
	io_out8(0x70,0x00); 
}
