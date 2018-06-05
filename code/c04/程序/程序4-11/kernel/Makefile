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

all: system
	objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary system kernel.bin

system:	head.o entry.o main.o printk.o trap.o memory.o interrupt.o task.o
	ld -b elf64-x86-64 -z muldefs -o system head.o entry.o main.o printk.o trap.o memory.o interrupt.o task.o -T Kernel.lds 

head.o:	head.S
	gcc -E  head.S > head.s
	as --64 -o head.o head.s

entry.o: entry.S
	gcc -E  entry.S > entry.s
	as --64 -o entry.o entry.s
	
main.o:	main.c
	gcc  -mcmodel=large -fno-builtin -m64 -c main.c
	
printk.o: printk.c
	gcc  -mcmodel=large -fno-builtin -m64 -c printk.c

trap.o: trap.c
	gcc  -mcmodel=large -fno-builtin -m64 -c trap.c

memory.o: memory.c
	gcc  -mcmodel=large -fno-builtin -m64 -c memory.c

interrupt.o: interrupt.c
	gcc  -mcmodel=large -fno-builtin -m64 -c interrupt.c
	
task.o: task.c
	gcc  -mcmodel=large -fno-builtin -m64 -c task.c	

clean:
	rm -rf *.o *.s~ *.s *.S~ *.c~ *.h~ system  Makefile~ Kernel.lds~ kernel.bin 

