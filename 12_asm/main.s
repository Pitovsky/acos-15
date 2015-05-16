%define sizeof(x) x %+ _size

section .data

    argv1 db "-w"

    argvusingstr db "WEAK CHEKING: ON", 10, 0
    argvusingstrlen equ $ - argvusingstr

    weak db 0

    sys_exit        equ     1
    sys_read        equ     3
    sys_write       equ     4
    sys_open		equ		5
    sys_close		equ		6
    sys_brk			equ		45
    sys_newstat		equ		106

    O_RDONLY		equ		0
    O_WRONLY		equ		1
    O_RDWR			equ		2

    stdin           equ     0
    stdout          equ     1
    stderr          equ     2

    struc STAT        
        .st_dev:        resd 1       
        .st_ino:        resd 1    
        .st_mode:       resw 1    
        .st_nlink:      resw 1    
        .st_uid:        resw 1    
        .st_gid:        resw 1    
        .st_rdev:       resd 1        
        .st_size:       resd 1    
        .st_blksize:    resd 1    
        .st_blocks:     resd 1    
        .st_atime:      resd 1    
        .st_atime_nsec: resd 1    
        .st_mtime:      resd 1    
        .st_mtime_nsec: resd 1
        .st_ctime:      resd 1    
        .st_ctime_nsec: resd 1    
        .unused4:       resd 1    
        .unused5:       resd 1    
    endstruc

%define sizeof(x) x %+ _size

section .bss

    string resb 0

section .text

%macro _syscall_3 4

	push edx    ;we don't want corrupt the data (:
	push ecx
	push ebx

	push %1
	push %2
	push %3
	push %4

	pop edx
	pop ecx
	pop ebx
	pop eax

	int 0x80

	pop ebx
	pop ecx
	pop edx

%endmacro

%macro _syscall_exit 1

	mov ebx, %1	; exit code
	mov eax, 1	; 1 = sys_exit
	int 0x80

%endmacro

%macro _syscall_read 3

	_syscall_3 3,%1,%2,%3

%endmacro

%macro _syscall_write 3

	_syscall_3 4,%1,%2,%3

%endmacro

%macro 	PRINT 1

	pusha
    pushf

	jmp %%astr

%%str	db	%1, 0
%%strln	equ	$ - %%str
%%astr:	_syscall_write 1, %%str, %%strln

	popf
	popa

%endmacro

%macro	PUTCHAR 1

	pusha
	pushf
%ifstr %1
	mov	al, %1
%elifnum %1
	mov	al, %1
%elifidni %1, al
	nop
%elifidni %1, ah
	mov	al, ah
%elifidni %1, bl
	mov	al, bl
%elifidni %1, bh
	mov	al, bh
%elifidni %1, cl
	mov	al, cl
%elifidni %1, ch
	mov	al, ch
%elifidni %1, dl
	mov	al, dl
%elifidni %1, dh
	mov	al, dh
%else
	mov	al, %1
%endif
	sub	esp, 2
	mov	edi, esp
	mov	[edi], al
	_syscall_write 1, edi, 1
	add	esp, 2
	popf
	popa

%endmacro

%macro GETCHAR 0

	pushf
	push	edi
	sub	esp, 2
	mov	edi, esp
	_syscall_read 0, edi, 1
	cmp	eax, 1
	jne	%%eof_reached
	xor	eax,eax
	mov	al, [edi]
	jmp	%%gcquit

%%eof_reached:

	xor	eax, eax
	not	eax

%%gcquit:

	add	esp, 2
	pop	edi
	popf

%endmacro

%macro FINISH 0-1 0
	_syscall_exit %1
%endmacro

global _start

_start:

    xor eax, eax

    mov edx, [esp + 8]
    mov ebx, [argv1]

    cmp bl, [edx]
    jne .not_equal

    mov edx, [esp + 8]
    mov ebx, [argv1 + 1]

    cmp bl, [edx + 1]
    jne .not_equal

    mov eax, 1
    mov [weak], eax

    mov eax, 4
    mov ebx, 1
    mov ecx, argvusingstr
    mov edx, argvusingstrlen - 1
    int 80h

    .not_equal:


    FINISH 0
