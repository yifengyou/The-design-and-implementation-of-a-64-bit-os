# 1 "head.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "head.S"
# 16 "head.S"
# 1 "linkage.h" 1
# 17 "head.S" 2

.section .text

.global _start; _start:

 mov $0x10, %ax
 mov %ax, %ds
 mov %ax, %es
 mov %ax, %fs
 mov %ax, %ss
 mov $0x7E00, %esp



 lgdt GDT_POINTER(%rip)



 lidt IDT_POINTER(%rip)

 mov $0x10, %ax
 mov %ax, %ds
 mov %ax, %es
 mov %ax, %fs
 mov %ax, %gs
 mov %ax, %ss

 movq $0x7E00, %rsp



 movq $0x101000, %rax
 movq %rax, %cr3
 movq switch_seg(%rip), %rax
 pushq $0x08
 pushq %rax
 lretq



switch_seg:
 .quad entry64

entry64:
 movq $0x10, %rax
 movq %rax, %ds
 movq %rax, %es
 movq %rax, %gs
 movq %rax, %ss
 movq _stack_start(%rip), %rsp

setup_IDT:
 leaq ignore_int(%rip), %rdx
 movq $(0x08 << 16), %rax
 movw %dx, %ax
 movq $(0x8E00 << 32), %rcx
 addq %rcx, %rax
 movl %edx, %ecx
 shrl $16, %ecx
 shlq $48, %rcx
 addq %rcx, %rax
 shrq $32, %rdx
 leaq IDT_Table(%rip), %rdi
 mov $256, %rcx
rp_sidt:
 movq %rax, (%rdi)
 movq %rdx, 8(%rdi)
 addq $0x10, %rdi
 dec %rcx
 jne rp_sidt

setup_TSS64:
 leaq TSS64_Table(%rip), %rdx
 xorq %rax, %rax
 xorq %rcx, %rcx
 movq $0x89, %rax
 shlq $40, %rax
 movl %edx, %ecx
 shrl $24, %ecx
 shlq $56, %rcx
 addq %rcx, %rax
 xorq %rcx, %rcx
 movl %edx, %ecx
 andl $0xffffff, %ecx
 shlq $16, %rcx
 addq %rcx, %rax
 addq $103, %rax
 leaq GDT_Table(%rip), %rdi
 movq %rax, 80(%rdi)
 shrq $32, %rdx
 movq %rdx, 88(%rdi)




 movq go_to_kernel(%rip), %rax
 pushq $0x08
 pushq %rax
 lretq

go_to_kernel:
 .quad Start_Kernel




ignore_int:
 cld
 pushq %rax
 pushq %rbx
 pushq %rcx
 pushq %rdx
 pushq %rbp
 pushq %rdi
 pushq %rsi

 pushq %r8
 pushq %r9
 pushq %r10
 pushq %r11
 pushq %r12
 pushq %r13
 pushq %r14
 pushq %r15

 movq %es, %rax
 pushq %rax
 movq %ds, %rax
 pushq %rax

 movq $0x10, %rax
 movq %rax, %ds
 movq %rax, %es

 leaq int_msg(%rip), %rax
 pushq %rax
 movq %rax, %rdx
 movq $0x00000000, %rsi
 movq $0x00ff0000, %rdi
 movq $0, %rax
 callq color_printk
 addq $0x8, %rsp

Loop:
 jmp Loop

 popq %rax
 movq %rax, %ds
 popq %rax
 movq %rax, %es

 popq %r15
 popq %r14
 popq %r13
 popq %r12
 popq %r11
 popq %r10
 popq %r9
 popq %r8

 popq %rsi
 popq %rdi
 popq %rbp
 popq %rdx
 popq %rcx
 popq %rbx
 popq %rax
 iretq

int_msg:
 .asciz "Unknown interrupt or fault at RIP\n"

.global _stack_start; _stack_start:
 .quad init_task_union + 32768




.align 8

.org 0x1000

__PML4E:

 .quad 0x102007
 .fill 255,8,0
 .quad 0x102007
 .fill 255,8,0

.org 0x2000

__PDPTE:

 .quad 0x103007
 .fill 511,8,0

.org 0x3000

__PDE:

 .quad 0x000087
 .quad 0x200087
 .quad 0x400087
 .quad 0x600087
 .quad 0x800087
 .quad 0xe0000087
 .quad 0xe0200087
 .quad 0xe0400087
 .quad 0xe0600087
 .quad 0xe0800087
 .quad 0xe0a00087
 .quad 0xe0c00087
 .quad 0xe0e00087
 .fill 499,8,0



.section .data

.globl GDT_Table

GDT_Table:
 .quad 0x0000000000000000
 .quad 0x0020980000000000
 .quad 0x0000920000000000
 .quad 0x0000000000000000
 .quad 0x0000000000000000
 .quad 0x0020f80000000000
 .quad 0x0000f20000000000
 .quad 0x00cf9a000000ffff
 .quad 0x00cf92000000ffff
 .fill 10,8,0
GDT_END:

GDT_POINTER:
GDT_LIMIT: .word GDT_END - GDT_Table - 1
GDT_BASE: .quad GDT_Table



.globl IDT_Table

IDT_Table:
 .fill 512,8,0
IDT_END:

IDT_POINTER:
IDT_LIMIT: .word IDT_END - IDT_Table - 1
IDT_BASE: .quad IDT_Table



.globl TSS64_Table

TSS64_Table:
 .fill 13,8,0
TSS64_END:


TSS64_POINTER:
TSS64_LIMIT: .word TSS64_END - TSS64_Table - 1
TSS64_BASE: .quad TSS64_Table
