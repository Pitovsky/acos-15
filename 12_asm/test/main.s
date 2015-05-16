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

sys_exit        equ     1
sys_read        equ     3
sys_write       equ     4
sys_open		equ		5
sys_close		equ		6
sys_brk			equ		45
sys_newstat		equ		106

EOF equ -1

O_RDONLY		equ		0
O_WRONLY		equ		1
O_RDWR			equ		2

stdin           equ     0
stdout          equ     1
stderr          equ     2

%define sizeof(x) x %+ _size

SECTION     .data
    
    weakArgument db "-w"

SECTION     .bss

    Org_Break   resd    1
    TempBuf		resd	1

    buffer_size resd    1

    is_weak     resb    1

SECTION     .text

global      _start

_start:

    ;check the first argument
    xor eax, eax

    mov edx, [esp]
    cmp dl, 2
    jne .weak_not_provided

    mov edx, [esp + 8]
    mov ebx, [weakArgument]

    cmp bl, [edx]
    jne .weak_not_provided

    mov edx, [esp + 8]
    mov ebx, [weakArgument + 1]

    cmp bl, [edx + 1]
    jne .weak_not_provided

    mov eax, 1
    mov [is_weak], eax

    PRINT "WEAK CHEKING: ON"
    PUTCHAR 10

    .weak_not_provided:

;    mov ebx, esp
;    mov ecx, [ebx]
;
;    add ebx, 4
;
;    argumentsLoop:
;
;        add ebx, 4
;
;    loop argumentsLoop
;
;    add ebx, 4
;
;    mov		ecx, [ebx]
;    mov		ebx, stdout
;	mov		edx, 102400
;	mov		eax, sys_write
;	int		0x80

    ; set buffer size

    push edx

    mov edx, 10
    mov [buffer_size], edx

    pop edx

	; Get the end of bss section
	xor		ebx, ebx
	mov		eax, sys_brk
	int		0x80
	mov		[Org_Break], eax
	mov		[TempBuf], eax
	push	eax
	
	; extend it by 1024 bytes
	pop		ebx
	add		ebx, 10                     ; <----------- initital_buffer_size
	mov		eax, sys_brk
	int		0x80
    push ebx

    mov ecx, dword 20                   ; <----------- buffer_size
    mov esi, [TempBuf]

    lp1:

        push ecx

        mov     ebx, stdin
        mov		ecx, esi
        mov		edx, dword 1
        mov		eax, sys_read
        int		0x80

        pop ecx

        mov edx, [esi]

        cmp dl, 0
        je end_of_file

        inc esi

    loop lp1

    mov ecx, [buffer_size]

    lp2:

        push ecx

        mov     ebx, stdin
        mov		ecx, esi
        mov		edx, dword 1
        mov		eax, sys_read
        int		0x80

        push edx

        mov edx, [esi]

        cmp dl, 0
        je end_of_file

        pop edx
        pop ecx

        inc esi

    loop lp2

	jmp END_OF_BUFFER

    end_of_file:

    PUTCHAR 10

    push dword [TempBuf]
    call main_magic
    add esp, 4

	; free memory
	mov     ebx, [Org_Break]
    mov     eax, sys_brk
    int     0x80
   	
Exit:

    mov     eax, sys_exit
    xor     ebx, ebx
    int     0x80

END_OF_BUFFER:

    PRINT "WASTED!!!111!11!1: BUFFER IS ENDED"
    PUTCHAR 10
    jmp Exit

main_magic:

    ;
    ; here we've got argument on the stack, so we keep going on
    ;

    mov ebp, esp
    add ebp, 4
    mov esi, [ebp]

    xor ecx, ecx

    str_loop:

        cmp dword [esi], 0
        je .end_of_str

        inc ecx
        inc esi

    jmp str_loop

    .end_of_str:

    mov edi, esi
    mov esi, [ebp]

    sub edi, 1

    push ecx
    push edi
    push esi

    .cmp_str_loop:

        mov eax, [edi]
        cmp al, [esi]
        jne not_polindrom

        dec edi
        inc esi

    loop .cmp_str_loop

    add esp, 12

    PRINT "YEP"
    PUTCHAR 10
    ret

not_polindrom:

    pop esi
    pop edi
    pop ecx

    cmp byte [is_weak], 0
    je .is_not_weak

    mov byte [is_weak], 0

    .cmp_weak_str_loop:

        push ecx

            .weak_processor_loop_esi:

                PRINT "loop"

                push 1
                push dword 1
                push esi
                call weak_processor
                pop esi
                add esp, 4
                pop eax

                add eax, 1
                mov ecx, eax

            loop .weak_processor_loop_esi

        pop ecx
        push ecx

            .weak_processor_loop_edi:

                push 1
                push dword 1
                push edi
                call weak_processor
                pop edi
                add esp, 4
                pop eax

                mov ecx, eax

            loop .weak_processor_loop_edi

        pop ecx

        mov eax, [edi]
        cmp al, [esi]
        jne .is_not_weak

        dec edi
        inc esi

    loop .cmp_weak_str_loop

    .is_weak:

        PRINT "WEAK"
        PUTCHAR 10

        ret

    .is_not_weak:

        PRINT "NOOPE"
        PUTCHAR 10

        ret

weak_processor:

    mov ebp, esp
    add ebp, 4

    mov eax, [ebp]

    cmp byte [eax], 32
    je .remove_weakness

    cmp byte [eax], 44
    je .remove_weakness

    cmp byte [eax], 46
    je .remove_weakness

    mov dword [ebp + 8], 0
    jmp .ord_exit

    .remove_weakness:

        dec ecx

        add byte [ebp], 1

        cmp byte [ebp + 4], 0
        je .second_var
        jmp .ord_exit

    .second_var:

        sub byte [ebp], 2

    .ord_exit:

        ret



