FILES = ./build/kernel.asm.o ./build/kernel.o
FLAGS = -g -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc -Wextra -fno-pie -fno-common

all:
	nasm -f bin ./bootloader/bootloader.asm -o ./bin/bootloader.bin
	nasm -f elf -g ./bootloader/kernel.asm -o ./build/kernel.asm.o
	i686-elf-gcc -I ./kernel $(FLAGS) -std=gnu99 -c ./kernel/kernel.c -o ./build/kernel.o
	i686-elf-ld -g -relocatable $(FILES) -o ./build/completeKernel.o
	i686-elf-gcc $(FLAGS) -T linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/completeKernel.o

	dd if=./bin/bootloader.bin >> os.bin  
	dd if=./bin/kernel.bin >> os.bin 
	dd if=/dev/zero bs=512 count=10 >> os.bin
	
clean:
	rm -f ./bin/bootloader.bin
	rm -f ./bin/kernel.bin
	rm -f  os.bin
	rm -f ./build/kernel.asm.o
	rm -f ./build/kernel.o
	rm -f ./build/completeKernel.o
	
run-Doors:
	qemu-system-x86_64 -drive format=raw,file=os.bin
