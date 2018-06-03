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

all: system_api_lib
	objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary system_api_lib init.bin

system_api_lib: init.o lib.o errno.o malloc.o printf.o string.o keyboard.o dirent.o
	ld -b elf64-x86-64 -z muldefs -o system_api_lib init.o lib.o errno.o malloc.o printf.o string.o keyboard.o dirent.o -T User.lds

init.o: init.c
	gcc  $(CFLAGS) -c init.c

lib.o: lib.c
	gcc  $(CFLAGS) -c lib.c

errno.o: errno.c
	gcc  $(CFLAGS) -c errno.c

malloc.o: malloc.c
	gcc  $(CFLAGS) -c malloc.c

printf.o: printf.c
	gcc  $(CFLAGS) -c printf.c

string.o: string.c
	gcc  $(CFLAGS) -c string.c

keyboard.o: keyboard.c
	gcc  $(CFLAGS) -c keyboard.c

dirent.o: dirent.c
	gcc  $(CFLAGS) -c dirent.c

clean:
	rm -rf *.o *.s~ *.s *.S~ *.c~ *.h~ Makefile~ User.lds~ system_api_lib init.bin

