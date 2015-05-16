all: main.o stdio.s
	ld -m elf_i386 -s $< -o asm

main.o: main.s
	nasm -f elf $<

clean:
	rm *.o asm
