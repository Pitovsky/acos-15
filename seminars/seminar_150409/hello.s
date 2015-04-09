.data
    greeting_str:# label for further data
        .string "Hello world!\n" # string to output
    
    .set str_length, . - greeting_str - 1
    .set sys_exit_num, 60    
    .set STDOUTFD, 1
    .set sys_write_num, 1

    print_onellint_f:
        .string "%lld\n"

    scanf_twollint_f:
        .string "%lld %lld"
.bss
    firstVar:
        .space 8
    secondVar:
        .space 8
.text
    .global main
    .type main, @function

    .type printnum, @function
    printnum:#
       #prolog of program
         pushq %rbp
         movq %rsp, %rbp 
       #save previos values of registers
         pushq %rax
         pushq %rsi
         pushq %rdi
       #printf(print_onellint_f, arg)        
            movq $print_onellint_f, %rdi
            movq 16(%rbp), %rsi
            movb $0, %al
            call printf                
      # restore previos values of registers
        popq %rdi
        popq %rsi
        popq %rax
      #epilog of program
        popq %rbp
        ret
      

        
    main:
        # write(1, greeting_str, str_length)
            movq $STDOUTFD, %rdi
            movq $greeting_str, %rsi
            movq $str_length, %rdx
            
            movq $sys_write_num, %rax
            syscall

        pushq $10
        call printnum    
        popq %rax

        # tmpvar = scanf(scanf_twollint_f, &firstVar, &secondVar)
            movq $scanf_twollint_f, %rdi
            movq $firstVar, %rsi
            movq $secondVar, %rdx
            movb $0, %al
            call scanf

        # print of tmpvar
            pushq %rax
            call printnum
            popq %rax
        
        cmp $2, %rax
        jne endprogram_fail

        # print of first and second var
            movq $firstVar, %rax
            pushq (%rax)
            call printnum
            movq $secondVar, %rax
            pushq (%rax)
            call printnum
            
            addq $16 , %rsp  # equal to two calls of popq

        # for(i = 3; i > 0 ; --i){ firstvar *= (seondVar+i-1);  }
            movq $3, %rcx

            movq $firstVar, %rbx
            movq (%rbx), %rax # store firstVar into %rax, and &firstVar into %fbx
            movq $secondVar, %rdi
            movq (%rdi), %rsi #store secondVar in rsi

            begin_of_loop:
                addq $1, %rsi
                mulq %rsi
            loop begin_of_loop

            movq %rax, (%rbx) #store result into first var
        # print firstVar
            movq $firstVar, %rax
            pushq (%rax)
            call printnum       
            popq %rax
        # _exit(0)    
            movq  $sys_exit_num , %rax
            movq $0, %rdi
            syscall

    endprogram_fail:
        # _exit(1)    
            movq  $sys_exit_num , %rax
            movq $1, %rdi
            syscall


