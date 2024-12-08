	.file	"main.c"
	.text
	.section	.rodata
.LC0:
	.string	"Remaining space: %ld\n"
.LC1:
	.string	"Remaining space (back): %ld\n"
	.text
	.globl	recfn
	.type	recfn, @function
recfn:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset 3, -24
	movl	%edi, -36(%rbp)
	movq	current_hs@gottpoff(%rip), %rax
	movq	%fs:(%rax), %rax
	movq	%rax, %rdi
	call	heapstack_remaining_space@PLT
	movq	%rax, %rsi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	cmpl	$0, -36(%rbp)
	je	.L2
	cmpl	$1, -36(%rbp)
	jne	.L3
.L2:
	movl	-36(%rbp), %eax
	jmp	.L4
.L3:
	movl	-36(%rbp), %eax
	subl	$1, %eax
	movl	%eax, %edi
	call	recfn
	movl	%eax, %ebx
	movl	-36(%rbp), %eax
	subl	$2, %eax
	movl	%eax, %edi
	call	recfn
	addl	%ebx, %eax
	cltq
	movq	%rax, -24(%rbp)
	movq	current_hs@gottpoff(%rip), %rax
	movq	%fs:(%rax), %rax
	movq	%rax, %rdi
	call	heapstack_remaining_space@PLT
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-24(%rbp), %rax
.L4:
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	recfn, .-recfn
	.section	.rodata
.LC2:
	.string	"Remaining space (call): %ld\n"
	.text
	.globl	heap_entry
	.type	heap_entry, @function
heap_entry:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	current_hs@gottpoff(%rip), %rax
	movq	%fs:(%rax), %rax
	movq	%rax, %rdi
	call	heapstack_remaining_space@PLT
	movq	%rax, %rsi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movl	%eax, %edi
	call	recfn
	movslq	%eax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	heap_entry, .-heap_entry
	.section	.rodata
.LC3:
	.string	"Usage: %s <number>\n"
.LC4:
	.string	"Fib(%ld): %ld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movl	%edi, -52(%rbp)
	movq	%rsi, -64(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	cmpl	$2, -52(%rbp)
	je	.L7
	movq	-64(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$1, %eax
	jmp	.L8
.L7:
	movq	-64(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atol@PLT
	movq	%rax, -24(%rbp)
	movl	$4096, %edi
	call	heapstack_new@PLT
	movq	%rax, -16(%rbp)
	movq	$0, -32(%rbp)
	jmp	.L9
.L10:
	movq	-32(%rbp), %rax
	movq	%rax, -40(%rbp)
	leaq	-40(%rbp), %rdx
	movq	-16(%rbp), %rax
	leaq	heap_entry(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	heapstack_call@PLT
	movq	-40(%rbp), %rdx
	movq	-32(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addq	$1, -32(%rbp)
.L9:
	movq	-32(%rbp), %rax
	cmpq	-24(%rbp), %rax
	jle	.L10
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	heapstack_free@PLT
	movl	$0, %eax
.L8:
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L11
	call	__stack_chk_fail@PLT
.L11:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.ident	"GCC: (GNU) 13.1.1 20230429"
	.section	.note.GNU-stack,"",@progbits
