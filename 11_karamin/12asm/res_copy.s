.data
        .set sys_exit_num, 60
msg_pal_ok:
        .string "pal OK\n"
msg_not_pal:
        .string "Not pal\n"
sformat:
        .string  "%s\n"
dformat:
        .string  "%d\n"
cformat:
        .string  "%c\n"
dynamic_msg:
        .string  "Dynamic started\n"
read_ok:
	.string	"Read str='%s' OK\n"
owerflow:
	.string	"Owerflow\n"
env_name:
	.string	"MAX_BUF"
weak_flag:
	.string	"-weak"
open_mode:
	.string	"r"
file_name:
	.string	"/home/avk/temp/Assembler/input.txt"
iformat:
        .string "myloop iter = %d\n"
temp:
        .string "symbol %c == symbol %c\n"

new_line:
	.byte	10
line_end:
	.zero  	1
iterator:
        .long    0
weak_check:
	.long	1
limit_static:
	.long	1024
limit_dynamic:
	.long	8192
len:
        .long    0
left:
        .long    0
right:
        .long    0
strnum_:
        .long    0
strnum:
        .long    0
.bss
mystr:
        .space   1024
current_string:
        .space   1025
dynamic_string_pointer:
        .space   8
input:
        .space   8
max_buf:
        .space   8

.text
    .globl main
    .type main, @function
    .type get, @function
    .type check_pal, @function
    .type static_limit, @function
    .type dynamic_limit, @function
main:
    movq %rsp, %rbp #for correct debugging
#Вытаскиваем из переменной окружения максимальный буффер
    movq $env_name, %rdi
    call getenv
    movq %rax, max_buf
#Если getenv != NULL limit_dynamic = atoi(max_buf);
    cmpq $0, %rax
    je open_file
#atoi(max_buf)
    movq $max_buf, %rdi
    movb $0, %al
    call atoi
#limit_dynamic = atoi(max_buf)
    movq %rax, limit_dynamic
#printf limit dynamic
    movq $dformat, %rdi
    movq limit_dynamic, %rsi
    movb $0, %al
    call printf   
open_file:
#FILE* input = fopen(file_name, open_mode);
    movq $file_name, %rdi
    movq $open_mode, %rsi
    movb $0, %al
    call fopen
    movq %rax, input

#fscanf(input, format, str);
    movq input, %rdi
    movq $dformat, %rsi
    movq $strnum, %rdx
    movb $0, %al
    call fscanf

while_begin:
#char* current_string = get(input);
#Вновь получаем следующую строку из файла input.txt
    pushq input
    call get
    popq input

#printf(read_ok, string);
#Выводим вновь считанную строку
    movq $read_ok, %rdi
    movq $current_string, %rsi
    movb $0, %al
    call printf

#check_pal()
    call check_pal
#Счётчик строк для ввода
    decq strnum
    movq $dformat, %rdi
    movq strnum, %rsi
    movb $0, %al
    call printf
    cmpq $0, strnum
    jne while_begin

#jmp while_begin
while_end:jmp finish

last_str:
    call check_pal
    
finish:
#fclose(input);
    movq input, %rdi
    movb $0, %al
    call fclose
ret                     
    

check_pal:
#prolog of program
    pushq %rbp
    movq %rsp, %rbp 
#save previos values of registers
    pushq %rax
    pushq %rsi
    pushq %rdi
check_begin:  
#strlen(current_string)  
    movq $current_string, %rdi
    movb $0, %al
    call strlen
    movq %rax, len
    movq len, %rbx
#strlen(current_string)
#i = 0; j = len - 1;
#Инициализируем left и right    
    movq $0, %rdx
    movq %rdx, left
    movq len, %rdx
    subq $1, %rdx
    movq %rdx, right        
pal:
#if (left > right) goto pal_ok;#
pushq %rdx             #
pushq %rbx             #
    movq (left), %rdi      #
    movq (right), %rsi      #
    cmpq %rdi, %rsi     #
popq %rbx              #
popq %rdx              #
    jg pal_ok;          #
#if (left > right) goto pal_ok;#

#if (current_string[left] != current_string[right]) \
#Положим адрес строки в rax
    movq $current_string, %rax
#получим символ в строке с номером left, результат в %dl    
    movq left, %rcx
    movb (%eax, %ecx), %dl
#получим символ в строке с номером right, результат в %dh    
    movq right, %rcx
    movb (%eax, %ecx), %dh
#если они разные - то не палиндром
    cmpb %dl, %dh
#Если символы не совпали, запускаем проверку на слабый палиндром
    jne check_weak
        
#Сдвигаем левый и правый счётчики    
    incq left
    decq right
#Выведем сравненный символ и перейдём в начало цикла
#    movq $cformat, %rdi
#    movb %dl, %sil
#    movb $0, %al
#    call printf
jmp pal
pal_end:    

check_weak:

#if (weak_check == 1) check_weak;
    movl $1, %eax
    cmpl %eax, weak_check
#Если проверять на слабый палиндром не нужно, то не палиндром
    jne not_pal
#Проверка соответственно левого и правого символов на "."; " "; ","; 
    movq $current_string, %rax
#Берем символ слева
    movq left, %rcx
    movb (%eax, %ecx), %dl
#С пробелом
    cmpb $0x20, %dl
    je to_pal_left
#С точкой
    cmpb $0x2e, %dl
    je to_pal_left
#С запятой
    cmpb $0x2c, %dl
    je to_pal_left
    
#Берем символ справа
    movq right, %rcx
    movb (%eax, %ecx), %dl
#С пробелом
    cmpb $0x20, %dl
    je to_pal_right
#С точкой
    cmpb $0x2e, %dl
    je to_pal_right
#С запятой
    cmpb $0x2c, %dl
    je to_pal_right
jmp check_weak_end
        
to_pal_left:
    incq left
    jmp pal
to_pal_right:
   decq right
    jmp pal

check_weak_end: jmp not_pal                
                    
pal_ok:
    movq $sformat, %rdi
    movq $msg_pal_ok, %rsi
    movb $0, %al
    call printf
    jmp check_end

not_pal:    movq $1, %rdx
    movq %rdx, weak_check
    movq $sformat, %rdi
    movq $msg_not_pal, %rsi
    movb $0, %al
    call printf
    jmp check_end
check_end: 
#restore previos values of registers
    popq %rdi
    popq %rsi
    popq %rax
#epilog of program
    popq %rbp                    
check_pal_end: ret




get:
#prolog of program
    pushq %rbp
    movq %rsp, %rbp 
#save previos values of registers
    pushq %rax
    pushq %rsi
    pushq %rdi
#get_begin
    movq 16(%rbp), %rdi
    movb $0, %al
    call fgetc
    
#Это будет указатель на текущий символ
    movq current_string, %rbx
    movq $0, iterator
loop_begin:
#Проверка на получение конца строки '\n'  
    cmpq $10, %rax
    je loop_end
#Проверка на переполнение статического лимита
    movq iterator, %rdx
    cmpq %rdx, limit_static
    jne skip_static
    call static_limit
#Проверка на переполнение динамического лимита
    cmpq %rdx, limit_dynamic
    call dynamic_limit
skip_static:
#ставим считанный символ на i-ый элемент строки    
#ставим терминатор на i+1-ый элемент строки    
    movq line_end, %rcx            
    movq $current_string, %rdi   
    addq iterator, %rdi
    movq %rax, (%rdi)
    movq %rcx, 1(%rdi)   

#Считываем символ    
    movq input, %rdi
    movb $0, %al
    call fgetc             
#Инкремент цикла
    addq $1, iterator  
    jmp loop_begin  
loop_end:
#get_end    

#restore previos values of registers
    popq %rdi
    popq %rsi
    popq %rax
#epilog of program
    popq %rbp
get_end: ret

static_limit:
pushq %rax
#Выделим динамическую память
    movq limit_dynamic, %rdi
    incq %rdi
    call malloc
    movq %rax, dynamic_string_pointer
#Скопируем старые данные
    movq dynamic_string_pointer, %rdi
    movq current_string, %rsi
    movq limit_static, %rdx
    call memcpy
#Переприсвоим указатели
    movq dynamic_string_pointer, %rdi
    movq %rdi, current_string
popq %rax
ret

dynamic_limit:
    movq $sformat, %rdi
    movq $owerflow, %rsi
    movb $0, %al
    call printf
    
    movq $sys_exit_num, %rax
    movq $1, %rdi
    syscall
ret
