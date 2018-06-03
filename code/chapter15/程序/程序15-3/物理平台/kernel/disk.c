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

#include "disk.h"
#include "APIC.h"
#include "memory.h"
#include "printk.h"
#include "lib.h"
#include "block.h"
#include "semaphore.h"

static int disk_flags = 0;

struct request_queue disk_request;

struct block_device_operation IDE_device_operation;

void end_request(struct block_buffer_node * node)
{
	if(node == NULL)
		color_printk(RED,BLACK,"end_request error\n");

	node->wait_queue.tsk->state = TASK_RUNNING;
	insert_task_queue(node->wait_queue.tsk);
//	node->wait_queue.tsk->flags |= NEED_SCHEDULE;
	current->flags |= NEED_SCHEDULE;

	kfree((unsigned long *)disk_request.in_using);
	disk_request.in_using = NULL;

	if(disk_request.block_request_count)
		cmd_out();
}

void add_request(struct block_buffer_node * node)
{
	list_add_to_before(&disk_request.wait_queue_list.wait_list,&node->wait_queue.wait_list);
	disk_request.block_request_count++;
}

long cmd_out()
{
	wait_queue_T *wait_queue_tmp = container_of(list_next(&disk_request.wait_queue_list.wait_list),wait_queue_T,wait_list);
	struct block_buffer_node * node = disk_request.in_using = container_of(wait_queue_tmp,struct block_buffer_node,wait_queue);
	list_del(&disk_request.in_using->wait_queue.wait_list);
	disk_request.block_request_count--;

	while(io_in8(PORT_DISK1_STATUS_CMD) & DISK_STATUS_BUSY)
		nop();

	switch(node->cmd)
	{
		case ATA_WRITE_CMD:	

			io_out8(PORT_DISK1_DEVICE,0x40);

			io_out8(PORT_DISK1_ERR_FEATURE,0);
			io_out8(PORT_DISK1_SECTOR_CNT,(node->count >> 8) & 0xff);
			io_out8(PORT_DISK1_SECTOR_LOW ,(node->LBA >> 24) & 0xff);
			io_out8(PORT_DISK1_SECTOR_MID ,(node->LBA >> 32) & 0xff);
			io_out8(PORT_DISK1_SECTOR_HIGH,(node->LBA >> 40) & 0xff);

			io_out8(PORT_DISK1_ERR_FEATURE,0);
			io_out8(PORT_DISK1_SECTOR_CNT,node->count & 0xff);
			io_out8(PORT_DISK1_SECTOR_LOW,node->LBA & 0xff);
			io_out8(PORT_DISK1_SECTOR_MID,(node->LBA >> 8) & 0xff);
			io_out8(PORT_DISK1_SECTOR_HIGH,(node->LBA >> 16) & 0xff);

			while(!(io_in8(PORT_DISK1_STATUS_CMD) & DISK_STATUS_READY))
				nop();
			io_out8(PORT_DISK1_STATUS_CMD,node->cmd);

			while(!(io_in8(PORT_DISK1_STATUS_CMD) & DISK_STATUS_REQ))
				nop();
			port_outsw(PORT_DISK1_DATA,node->buffer,256);
			break;

		case ATA_READ_CMD:

			io_out8(PORT_DISK1_DEVICE,0x40);

			io_out8(PORT_DISK1_ERR_FEATURE,0);
			io_out8(PORT_DISK1_SECTOR_CNT,(node->count >> 8) & 0xff);
			io_out8(PORT_DISK1_SECTOR_LOW ,(node->LBA >> 24) & 0xff);
			io_out8(PORT_DISK1_SECTOR_MID ,(node->LBA >> 32) & 0xff);
			io_out8(PORT_DISK1_SECTOR_HIGH,(node->LBA >> 40) & 0xff);

			io_out8(PORT_DISK1_ERR_FEATURE,0);
			io_out8(PORT_DISK1_SECTOR_CNT,node->count & 0xff);
			io_out8(PORT_DISK1_SECTOR_LOW,node->LBA & 0xff);
			io_out8(PORT_DISK1_SECTOR_MID,(node->LBA >> 8) & 0xff);
			io_out8(PORT_DISK1_SECTOR_HIGH,(node->LBA >> 16) & 0xff);

			while(!(io_in8(PORT_DISK1_STATUS_CMD) & DISK_STATUS_READY))
				nop();
			io_out8(PORT_DISK1_STATUS_CMD,node->cmd);
			break;
			
		case GET_IDENTIFY_DISK_CMD:

			io_out8(PORT_DISK1_DEVICE,0xe0);
			
			io_out8(PORT_DISK1_ERR_FEATURE,0);
			io_out8(PORT_DISK1_SECTOR_CNT,node->count & 0xff);
			io_out8(PORT_DISK1_SECTOR_LOW,node->LBA & 0xff);
			io_out8(PORT_DISK1_SECTOR_MID,(node->LBA >> 8) & 0xff);
			io_out8(PORT_DISK1_SECTOR_HIGH,(node->LBA >> 16) & 0xff);

			while(!(io_in8(PORT_DISK1_STATUS_CMD) & DISK_STATUS_READY))
				nop();			
			io_out8(PORT_DISK1_STATUS_CMD,node->cmd);

		default:
			color_printk(BLACK,WHITE,"ATA CMD Error\n");
			break;
	}
	return 1;
}

void read_handler(unsigned long nr, unsigned long parameter)
{
	struct block_buffer_node * node = ((struct request_queue *)parameter)->in_using;
	
	if(io_in8(PORT_DISK1_STATUS_CMD) & DISK_STATUS_ERROR)
		color_printk(RED,BLACK,"read_handler:%#010x\n",io_in8(PORT_DISK1_ERR_FEATURE));
	else
		port_insw(PORT_DISK1_DATA,node->buffer,256);

	node->count--;
	if(node->count)
	{
		node->buffer += 512;
		return;
	}

	end_request(node);
}

void write_handler(unsigned long nr, unsigned long parameter)
{
	struct block_buffer_node * node = ((struct request_queue *)parameter)->in_using;

	if(io_in8(PORT_DISK1_STATUS_CMD) & DISK_STATUS_ERROR)
		color_printk(RED,BLACK,"write_handler:%#010x\n",io_in8(PORT_DISK1_ERR_FEATURE));

	node->count--;
	if(node->count)
	{
		node->buffer += 512;
		while(!(io_in8(PORT_DISK1_STATUS_CMD) & DISK_STATUS_REQ))
			nop();
		port_outsw(PORT_DISK1_DATA,node->buffer,256);
		return;
	}

	end_request(node);
}

void other_handler(unsigned long nr, unsigned long parameter)
{
	struct block_buffer_node * node = ((struct request_queue *)parameter)->in_using;

	if(io_in8(PORT_DISK1_STATUS_CMD) & DISK_STATUS_ERROR)
		color_printk(RED,BLACK,"other_handler:%#010x\n",io_in8(PORT_DISK1_ERR_FEATURE));
	else
		port_insw(PORT_DISK1_DATA,node->buffer,256);

	end_request(node);
}

struct block_buffer_node * make_request(long cmd,unsigned long blocks,long count,unsigned char * buffer)
{
	struct block_buffer_node * node = (struct block_buffer_node *)kmalloc(sizeof(struct block_buffer_node),0);
	wait_queue_init(&node->wait_queue,current);

	switch(cmd)
	{
		case ATA_READ_CMD:
			node->end_handler = read_handler;
			node->cmd = ATA_READ_CMD;
			break;

		case ATA_WRITE_CMD:
			node->end_handler = write_handler;
			node->cmd = ATA_WRITE_CMD;
			break;

		default:///
			node->end_handler = other_handler;
			node->cmd = cmd;
			break;
	}
	
	node->LBA = blocks;
	node->count = count;
	node->buffer = buffer;

	return node;
}

void submit(struct block_buffer_node * node)
{	
	add_request(node);
	
	if(disk_request.in_using == NULL)
		cmd_out();
}

void wait_for_finish()
{
	current->state = TASK_UNINTERRUPTIBLE;
	schedule();
}

long IDE_open()
{
	color_printk(BLACK,WHITE,"DISK1 Opened\n");
	return 1;
}

long IDE_close()
{
	color_printk(BLACK,WHITE,"DISK1 Closed\n");
	return 1;
}

long IDE_ioctl(long cmd,long arg)
{
	struct block_buffer_node * node = NULL;
	
	if(cmd == GET_IDENTIFY_DISK_CMD)
	{
		node = make_request(cmd,0,0,(unsigned char *)arg);
		submit(node);
		wait_for_finish();
		return 1;
	}
	
	return 0;
}

long IDE_transfer(long cmd,unsigned long blocks,long count,unsigned char * buffer)
{
	struct block_buffer_node * node = NULL;
	if(cmd == ATA_READ_CMD || cmd == ATA_WRITE_CMD)
	{
		node = make_request(cmd,blocks,count,buffer);
		submit(node);
		wait_for_finish();
	}
	else
	{
		return 0;
	}
	
	return 1;
}

struct block_device_operation IDE_device_operation = 
{
	.open = IDE_open,
	.close = IDE_close,
	.ioctl = IDE_ioctl,
	.transfer = IDE_transfer,
};

hw_int_controller disk_int_controller = 
{
	.enable = IOAPIC_enable,
	.disable = IOAPIC_disable,
	.install = IOAPIC_install,
	.uninstall = IOAPIC_uninstall,
	.ack = IOAPIC_edge_ack,
};

void disk_handler(unsigned long nr, unsigned long parameter, struct pt_regs * regs)
{
	struct block_buffer_node * node = ((struct request_queue *)parameter)->in_using;
//	color_printk(BLACK,WHITE,"disk_handler,preempt:%d,pid:%d,cRSP:%#018lx,RSP:%#018lx,RIP:%#018lx\n",current->preempt_count,current->pid,get_rsp(),regs->rsp,regs->rip);
	node->end_handler(nr,parameter);
}

void disk_init()
{
	struct IO_APIC_RET_entry entry;

	entry.vector = 0x2f;
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

	register_irq(0x2f, &entry , &disk_handler, (unsigned long)&disk_request, &disk_int_controller, "disk1");

	io_out8(PORT_DISK1_ALT_STA_CTL,0);
	
	wait_queue_init(&disk_request.wait_queue_list,NULL);
	disk_request.in_using = NULL;
	disk_request.block_request_count = 0;
}

void disk_exit()
{
	unregister_irq(0x2f);
}
