	.file	"example.c"
	.section	.rodata
.LC0:
	.string	"%lld %lld %lld \n"
	.text
.globl main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$40, %rsp
	movq	$10, -40(%rbp)
	movq	$15, -24(%rbp)
	movq	-40(%rbp), %rax
	movq	-24(%rbp), %rcx
	movq	%rax, %rdx
#APP
# 8 "example.c" 1
	movq %rdx,%rax
mulq %rcx
movq %rax, %rax

# 0 "" 2
#NO_APP
	movq	%rax, -32(%rbp)
	movl	$.LC0, %eax
	movq	-24(%rbp), %rcx
	movq	-32(%rbp), %rdx
	movq	-40(%rbp), %rbx
	.cfi_offset 3, -24
	movq	%rbx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	addq	$40, %rsp
	popq	%rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.6 20110731 (Red Hat 4.4.6-3)"
	.section	.note.GNU-stack,"",@progbits
