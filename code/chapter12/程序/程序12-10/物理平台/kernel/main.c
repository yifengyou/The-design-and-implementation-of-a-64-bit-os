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

#include "lib.h"
#include "printk.h"
#include "gate.h"
#include "trap.h"
#include "memory.h"
#include "task.h"
#include "cpu.h"

#if  APIC
#include "APIC.h"
#else
#include "8259A.h"
#endif

#include "keyboard.h"
#include "mouse.h"
#include "disk.h"
#include "SMP.h"
#include "spinlock.h"
#include "time.h"

/*
		static var 
*/

struct Global_Memory_Descriptor memory_management_struct = {{0},0};

int global_i = 0;

extern spinlock_T SMP_lock;

void Start_Kernel(void)
{
	struct INT_CMD_REG icr_entry;
	unsigned int * tss = NULL;
	struct time time;
	
	Pos.XResolution = 1024;
	Pos.YResolution = 768;

	Pos.XPosition = 0;
	Pos.YPosition = 0;

	Pos.XCharSize = 8;
	Pos.YCharSize = 16;

	Pos.FB_addr = (int *)0xffff800003000000;
	Pos.FB_length = (Pos.XResolution * Pos.YResolution * 4 + PAGE_4K_SIZE - 1) & PAGE_4K_MASK;

	spin_init(&Pos.printk_lock);

	load_TR(10);

	set_tss64(TSS64_Table,_stack_start, _stack_start, _stack_start, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00);

	sys_vector_init();

	init_cpu();
	
	memory_management_struct.start_code = (unsigned long)& _text;
	memory_management_struct.end_code   = (unsigned long)& _etext;
	memory_management_struct.end_data   = (unsigned long)& _edata;
	memory_management_struct.end_brk    = (unsigned long)& _end;

	color_printk(RED,BLACK,"memory init \n");
	init_memory();

	color_printk(RED,BLACK,"slab init \n");
	slab_init();

	color_printk(RED,BLACK,"frame buffer init \n");
	frame_buffer_init();
	color_printk(WHITE,BLACK,"frame_buffer_init() is OK \n");

	color_printk(RED,BLACK,"pagetable init \n");	
	pagetable_init();
		
	color_printk(RED,BLACK,"interrupt init \n");

	#if  APIC
		APIC_IOAPIC_init();
	#else
		init_8259A();
	#endif

	get_cmos_time(&time);

	color_printk(RED,BLACK,"year:%#010x,month:%#010x,day:%#010x,hour:%#010x,mintue:%#010x,second:%#010x\n",time.year,time.month,time.day,time.hour,time.minute,time.second);

	color_printk(RED,BLACK,"keyboard init \n");
	keyboard_init();

	color_printk(RED,BLACK,"mouse init \n");
	mouse_init();

	color_printk(RED,BLACK,"ICR init \n");	

	SMP_init();

	//prepare send INIT IPI
	
	icr_entry.vector = 0x00;
	icr_entry.deliver_mode =  APIC_ICR_IOAPIC_INIT;
	icr_entry.dest_mode = ICR_IOAPIC_DELV_PHYSICAL;
	icr_entry.deliver_status = APIC_ICR_IOAPIC_Idle;
	icr_entry.res_1 = 0;
	icr_entry.level = ICR_LEVEL_DE_ASSERT;
	icr_entry.trigger = APIC_ICR_IOAPIC_Edge;
	icr_entry.res_2 = 0;
	icr_entry.dest_shorthand = ICR_ALL_EXCLUDE_Self;
	icr_entry.res_3 = 0;
	icr_entry.destination.x2apic_destination = 0x00;
	
	wrmsr(0x830,*(unsigned long *)&icr_entry);	//INIT IPI

	//prepare send Start-up IPI

	for(global_i = 1;global_i < 4;global_i++)
	{
		spin_lock(&SMP_lock);

		_stack_start = (unsigned long)kmalloc(STACK_SIZE,0) + STACK_SIZE;
		tss = (unsigned int *)kmalloc(128,0);
		set_tss_descriptor(10 + global_i * 2,tss);
		set_tss64(tss,_stack_start,_stack_start,_stack_start,_stack_start,_stack_start,_stack_start,_stack_start,_stack_start,_stack_start,_stack_start);
	
		icr_entry.vector = 0x20;
		icr_entry.deliver_mode = ICR_Start_up;
		icr_entry.dest_shorthand = ICR_No_Shorthand;
		icr_entry.destination.x2apic_destination = global_i;
	
		wrmsr(0x830,*(unsigned long *)&icr_entry);	//Start-up IPI
		wrmsr(0x830,*(unsigned long *)&icr_entry);	//Start-up IPI
	}

	icr_entry.vector = 0xc8;
	icr_entry.destination.x2apic_destination = 1;
	icr_entry.deliver_mode = APIC_ICR_IOAPIC_Fixed;

	wrmsr(0x830,*(unsigned long *)&icr_entry);

	icr_entry.vector = 0xc9;
	wrmsr(0x830,*(unsigned long *)&icr_entry);

	while(1)
	{
		if(p_kb->count)
			analysis_keycode();
		if(p_mouse->count)
			analysis_mousecode();
	}
}
