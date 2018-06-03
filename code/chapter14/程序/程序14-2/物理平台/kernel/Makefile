#/***************************************************
#		版权声明
#
#	本操作系统名为：MINE
#	该操作系统未经授权不得以盈利或非盈利为目的进行开发，
#	只允许个人学习以及公开交流使用
#
#	代码最终所有权及解释权归田宇所有；
#
#	本模块作者：	田宇
#	EMail:		345538255@qq.com
#
#
#***************************************************/

CFLAGS := -mcmodel=large -fno-builtin -m64

ASFLAGS := --64

PIC := APIC

all: system
	objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary system kernel.bin

system:	head.o entry.o APU_boot.o main.o printk.o trap.o memory.o interrupt.o PIC.o task.o cpu.o keyboard.o mouse.o disk.o SMP.o time.o HPET.o softirq.o timer.o schedule.o fat32.o VFS.o sys.o syscalls.o
	ld -b elf64-x86-64 -z muldefs -o system head.o entry.o APU_boot.o main.o printk.o trap.o memory.o interrupt.o PIC.o task.o cpu.o keyboard.o mouse.o disk.o SMP.o time.o HPET.o softirq.o timer.o schedule.o fat32.o VFS.o sys.o syscalls.o -T Kernel.lds 

head.o:	head.S
	gcc -E head.S > head.s
	as $(ASFLAGS) -o head.o head.s

entry.o: entry.S
	gcc -E entry.S > entry.s
	as $(ASFLAGS) -o entry.o entry.s

APU_boot.o: APU_boot.S
	gcc -E APU_boot.S > APU_boot.s
	as $(ASFLAGS) -o APU_boot.o APU_boot.s

main.o:	main.c
	gcc  $(CFLAGS) -c main.c -D$(PIC)

printk.o: printk.c
	gcc  $(CFLAGS) -c printk.c

trap.o: trap.c
	gcc  $(CFLAGS) -c trap.c

memory.o: memory.c
	gcc  $(CFLAGS) -c memory.c

interrupt.o: interrupt.c
	gcc  $(CFLAGS) -c interrupt.c

ifeq ($(PIC),APIC)
PIC.o: APIC.c
	gcc  $(CFLAGS) -c APIC.c -o PIC.o
else
PIC.o: 8259A.c
	gcc  $(CFLAGS) -c 8259A.c -o PIC.o
endif
	
task.o: task.c
	gcc  $(CFLAGS) -c task.c	

cpu.o: cpu.c
	gcc  $(CFLAGS) -c cpu.c

keyboard.o: keyboard.c
	gcc  $(CFLAGS) -c keyboard.c

mouse.o: mouse.c
	gcc  $(CFLAGS) -c mouse.c

disk.o: disk.c
	gcc  $(CFLAGS) -c disk.c

SMP.o: SMP.c
	gcc  $(CFLAGS) -c SMP.c

time.o: time.c
	gcc  $(CFLAGS) -c time.c

HPET.o: HPET.c
	gcc  $(CFLAGS) -c HPET.c

softirq.o: softirq.c
	gcc  $(CFLAGS) -c softirq.c

timer.o: timer.c
	gcc  $(CFLAGS) -c timer.c

schedule.o: schedule.c
	gcc  $(CFLAGS) -c schedule.c

fat32.o: fat32.c
	gcc  $(CFLAGS) -c fat32.c

VFS.o: VFS.c
	gcc  $(CFLAGS) -c VFS.c

sys.o: sys.c
	gcc  $(CFLAGS) -c sys.c

syscalls.o: syscalls.c
	gcc  $(CFLAGS) -c syscalls.c

clean:
	rm -rf *.o *.s~ *.s *.S~ *.c~ *.h~ system  Makefile~ Kernel.lds~ kernel.bin 

