# 1 "entry.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "entry.S"
# 16 "entry.S"
# 1 "linkage.h" 1
# 17 "entry.S" 2

R15 = 0x00
R14 = 0x08
R13 = 0x10
R12 = 0x18
R11 = 0x20
R10 = 0x28
R9 = 0x30
R8 = 0x38
RBX = 0x40
RCX = 0x48
RDX = 0x50
RSI = 0x58
RDI = 0x60
RBP = 0x68
DS = 0x70
ES = 0x78
RAX = 0x80
FUNC = 0x88
ERRCODE = 0x90
RIP = 0x98
CS = 0xa0
RFLAGS = 0xa8
OLDRSP = 0xb0
OLDSS = 0xb8

RESTORE_ALL:
 popq %r15;
 popq %r14;
 popq %r13;
 popq %r12;
 popq %r11;
 popq %r10;
 popq %r9;
 popq %r8;
 popq %rbx;
 popq %rcx;
 popq %rdx;
 popq %rsi;
 popq %rdi;
 popq %rbp;
 popq %rax;
 movq %rax, %ds;
 popq %rax;
 movq %rax, %es;
 popq %rax;
 addq $0x10, %rsp;
 iretq;





ret_from_exception:

.global ret_from_intr; ret_from_intr:
 jmp RESTORE_ALL

.global divide_error; divide_error:
 pushq $0
 pushq %rax
 leaq do_divide_error(%rip), %rax
 xchgq %rax, (%rsp)

error_code:
 pushq %rax
 movq %es, %rax
 pushq %rax
 movq %ds, %rax
 pushq %rax
 xorq %rax, %rax

 pushq %rbp
 pushq %rdi
 pushq %rsi
 pushq %rdx
 pushq %rcx
 pushq %rbx
 pushq %r8
 pushq %r9
 pushq %r10
 pushq %r11
 pushq %r12
 pushq %r13
 pushq %r14
 pushq %r15

 cld
 movq ERRCODE(%rsp), %rsi
 movq FUNC(%rsp), %rdx

 movq $0x10, %rdi
 movq %rdi, %ds
 movq %rdi, %es

 movq %rsp, %rdi


 callq *%rdx

 jmp ret_from_exception

.global debug; debug:
 pushq $0
 pushq %rax
 leaq do_debug(%rip), %rax
 xchgq %rax, (%rsp)
 jmp error_code

.global nmi; nmi:
 pushq %rax
 cld;
 pushq %rax;

 pushq %rax
 movq %es, %rax
 pushq %rax
 movq %ds, %rax
 pushq %rax
 xorq %rax, %rax

 pushq %rbp;
 pushq %rdi;
 pushq %rsi;
 pushq %rdx;
 pushq %rcx;
 pushq %rbx;
 pushq %r8;
 pushq %r9;
 pushq %r10;
 pushq %r11;
 pushq %r12;
 pushq %r13;
 pushq %r14;
 pushq %r15;

 movq $0x10, %rdx;
 movq %rdx, %ds;
 movq %rdx, %es;

 movq $0, %rsi
 movq %rsp, %rdi

 callq do_nmi

 jmp RESTORE_ALL

.global int3; int3:
 pushq $0
 pushq %rax
 leaq do_int3(%rip), %rax
 xchgq %rax, (%rsp)
 jmp error_code

.global overflow; overflow:
 pushq $0
 pushq %rax
 leaq do_overflow(%rip), %rax
 xchgq %rax, (%rsp)
 jmp error_code

.global bounds; bounds:
 pushq $0
 pushq %rax
 leaq do_bounds(%rip), %rax
 xchgq %rax, (%rsp)
 jmp error_code

.global undefined_opcode; undefined_opcode:
 pushq $0
 pushq %rax
 leaq do_undefined_opcode(%rip), %rax
 xchgq %rax, (%rsp)
 jmp error_code

.global dev_not_available; dev_not_available:
 pushq $0
 pushq %rax
 leaq do_dev_not_available(%rip), %rax
 xchgq %rax, (%rsp)
 jmp error_code


.global double_fault; double_fault:
 pushq %rax
 leaq do_double_fault(%rip), %rax
 xchgq %rax, (%rsp)
 jmp error_code

.global coprocessor_segment_overrun; coprocessor_segment_overrun:
 pushq $0
 pushq %rax
 leaq do_coprocessor_segment_overrun(%rip), %rax
 xchgq %rax, (%rsp)
 jmp error_code

.global invalid_TSS; invalid_TSS:
 pushq %rax
 leaq do_invalid_TSS(%rip), %rax
 xchgq %rax, (%rsp)
 jmp error_code

.global segment_not_present; segment_not_present:
 pushq %rax
 leaq do_segment_not_present(%rip), %rax
 xchgq %rax, (%rsp)
 jmp error_code

.global stack_segment_fault; stack_segment_fault:
 pushq %rax
 leaq do_stack_segment_fault(%rip), %rax
 xchgq %rax, (%rsp)
 jmp error_code

.global general_protection; general_protection:
 pushq %rax
 leaq do_general_protection(%rip), %rax
 xchgq %rax, (%rsp)
 jmp error_code

.global page_fault; page_fault:
 pushq %rax
 leaq do_page_fault(%rip), %rax
 xchgq %rax, (%rsp)
 jmp error_code

.global x87_FPU_error; x87_FPU_error:
 pushq $0
 pushq %rax
 leaq do_x87_FPU_error(%rip), %rax
 xchgq %rax, (%rsp)
 jmp error_code

.global alignment_check; alignment_check:
 pushq %rax
 leaq do_alignment_check(%rip), %rax
 xchgq %rax, (%rsp)
 jmp error_code

.global machine_check; machine_check:
 pushq $0
 pushq %rax
 leaq do_machine_check(%rip), %rax
 xchgq %rax, (%rsp)
 jmp error_code

.global SIMD_exception; SIMD_exception:
 pushq $0
 pushq %rax
 leaq do_SIMD_exception(%rip), %rax
 xchgq %rax, (%rsp)
 jmp error_code

.global virtualization_exception; virtualization_exception:
 pushq $0
 pushq %rax
 leaq do_virtualization_exception(%rip), %rax
 xchgq %rax, (%rsp)
 jmp error_code
