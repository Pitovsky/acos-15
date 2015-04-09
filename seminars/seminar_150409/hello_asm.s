	.file	"hello.c"
.globl str
	.data
	.type	str, @object
	.size	str, 13
str:
	.string	"Hello world!"
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
	movl	$13, %edx
	movl	$str, %esi
	movl	$1, %edi
	call	write
	movl	$0, %edi
	call	_exit
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.6 20110731 (Red Hat 4.4.6-3)"
	.section	.note.GNU-stack,"",@progbits
