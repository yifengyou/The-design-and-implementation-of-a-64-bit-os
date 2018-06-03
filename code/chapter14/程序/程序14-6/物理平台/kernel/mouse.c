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

#include "mouse.h"
#include "keyboard.h"
#include "APIC.h"
#include "memory.h"
#include "printk.h"

struct keyboard_inputbuffer * p_mouse = NULL;
static int mouse_count = 0;
struct mouse_packet mouse;

/*

*/

void mouse_handler(unsigned long nr, unsigned long parameter, struct pt_regs * regs)
{
	unsigned char x;
	x = io_in8(PORT_KB_DATA);
	color_printk(GREEN,WHITE,"(M:%02x)",x);

	if(p_mouse->p_head == p_mouse->buf + KB_BUF_SIZE)
		p_mouse->p_head = p_mouse->buf;

	*p_mouse->p_head = x;
	p_mouse->count++;
	p_mouse->p_head ++;
}

/*

*/

unsigned char get_mousecode()
{
	unsigned char ret  = 0;

	if(p_mouse->count == 0)
		while(!p_mouse->count)
			nop();
	
	if(p_mouse->p_tail == p_mouse->buf + KB_BUF_SIZE)	
		p_mouse->p_tail = p_mouse->buf;

	ret = *p_mouse->p_tail;
	p_mouse->count--;
	p_mouse->p_tail++;

	return ret;
}

/*

*/

void analysis_mousecode()
{
	unsigned char x = get_mousecode();

	switch(mouse_count)
	{
		case 0:
			mouse_count++;
			break;

		case 1:
			mouse.Byte0 = x;
			mouse_count++;
			break;
		
		case 2:
			mouse.Byte1 = (char)x;
			mouse_count++;
			break;

		case 3:
			mouse.Byte2 = (char)x;
			mouse_count = 1;
			color_printk(RED,GREEN,"(M:%02x,X:%3d,Y:%3d)\n",mouse.Byte0,mouse.Byte1,mouse.Byte2);
			break;

		default:			
			break;
	}
}

hw_int_controller mouse_int_controller = 
{
	.enable = IOAPIC_enable,
	.disable = IOAPIC_disable,
	.install = IOAPIC_install,
	.uninstall = IOAPIC_uninstall,
	.ack = IOAPIC_edge_ack,
};


/*

*/

void mouse_init()
{
	struct IO_APIC_RET_entry entry;
	unsigned long i,j;

	p_mouse = (struct keyboard_inputbuffer *)kmalloc(sizeof(struct keyboard_inputbuffer),0);
	
	p_mouse->p_head = p_mouse->buf;
	p_mouse->p_tail = p_mouse->buf;
	p_mouse->count  = 0;
	memset(p_mouse->buf,0,KB_BUF_SIZE);

	entry.vector = 0x2c;
	entry.deliver_mode = APIC_ICR_IOAPIC_Fixed ;
	entry.dest_mode = ICR_IOAPIC_DELV_PHYSICAL;
	entry.deliver_status = APIC_ICR_IOAPIC_Idle;
	entry.polarity = APIC_IOAPIC_POLARITY_HIGH;
	entry.irr = APIC_IOAPIC_IRR_RESET;
	entry.trigger = APIC_ICR_IOAPIC_Edge;
	entry.mask = APIC_ICR_IOAPIC_Masked;
	entry.reserved = 0;

	entry.destination.physical.reserved1 = 0;
	entry.destination.physical.phy_dest = 0;
	entry.destination.physical.reserved2 = 0;

	mouse_count = 0;

	register_irq(0x2c, &entry , &mouse_handler, (unsigned long)p_mouse, &mouse_int_controller, "ps/2 mouse");

	wait_KB_write();
	io_out8(PORT_KB_CMD,KBCMD_EN_MOUSE_INTFACE);

	for(i = 0;i<1000;i++)
		for(j = 0;j<1000;j++)
			nop();

	wait_KB_write();
	io_out8(PORT_KB_CMD,KBCMD_SENDTO_MOUSE);
	wait_KB_write();
	io_out8(PORT_KB_DATA,MOUSE_ENABLE);

	for(i = 0;i<1000;i++)
		for(j = 0;j<1000;j++)
			nop();

	wait_KB_write();
	io_out8(PORT_KB_CMD,KBCMD_WRITE_CMD);
	wait_KB_write();
	io_out8(PORT_KB_DATA,KB_INIT_MODE);
}

/*

*/

void mouse_exit()
{
	unregister_irq(0x2c);
	kfree((unsigned long *)p_mouse);
}
