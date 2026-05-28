FILES = ./build/kernel.asm.o ./build/kernel.o ./build/VGA.o ./build/stdio.o ./build/string.o ./build/mem.o ./build/ports.o ./build/8259_PIC.o ./build/isr.asm.o ./build/isr.o ./build/irq.asm.o ./build/IDT.o
FLAGS = -g -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc -Wextra -fno-pie -fno-common

all:
	nasm -f bin ./bootloader/bootloader.asm -o ./bin/bootloader.bin
	nasm -f elf -g ./bootloader/kernel.asm -o ./build/kernel.asm.o
	i686-elf-gcc -I ./kernel $(FLAGS) -std=gnu99 -c ./kernel/kernel.c -o ./build/kernel.o
	i686-elf-gcc -I ./drivers/ports $(FLASGS) -std=gnu99 -c ./drivers/ports/ports.c -o ./build/ports.o
	i686-elf-gcc -I ./drivers/VGA $(FLAGS) -std=gnu99 -c ./drivers/VGA/VGA.c -o ./build/VGA.o
	i686-elf-gcc -I ./headers/stdio $(FLAGS) -std=gnu99 -c ./headers/stdio/stdio.c -o ./build/stdio.o
	i686-elf-gcc -I ./headers/string $(FLAGS) -std=gnu99 -c ./headers/string/string.c -o ./build/string.o
	i686-elf-gcc -I ./CPU/IDT $(FLAGS) -std=gnu99 -c ./CPU/IDT/IDT.c -o ./build/IDT.o
	i686-elf-gcc -I ./headers/mem $(FLAGS) -std=gnu99 -c ./headers/mem/mem.c -o ./build/mem.o
	i686-elf-gcc -I ./CPU/8259_PIC $(FLAGS) -std=gnu99 -c ./CPU/8259_PIC/8259_PIC.c -o ./build/8259_PIC.o
	nasm -f elf -g ./CPU/ISR_IRQ/isr.asm -o ./build/isr.asm.o 
	nasm -f elf -g ./CPU/ISR_IRQ/irq.asm -o ./build/irq.asm.o 
	i686-elf-gcc -I ./CPU/ISR_IRQ $(FLAGS) -std=gnu99 -c ./CPU/ISR_IRQ/isr.c -o ./build/isr.o
	i686-elf-ld -g -relocatable $(FILES) -o ./build/completeKernel.o
	i686-elf-gcc $(FLAGS) -T linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/completeKernel.o

	dd if=./bin/bootloader.bin >> DoorsOS.bin  
	dd if=./bin/kernel.bin >> DoorsOS.bin 
	dd if=/dev/zero bs=512 count=10 >> DoorsOS.bin
	#img fajl kreacija
	dd if=/dev/zero of=DoorsOS.img bs=1M count=16
	dd if=./bin/bootloader.bin of=DoorsOS.img conv=notrunc
	dd if=./bin/kernel.bin of=DoorsOS.img bs=512 seek=1 conv=notrunc
	
clean:
	rm -f ./bin/bootloader.bin
	rm -f ./bin/kernel.bin
	rm -f  DoorsOS.bin
	rm -f ./build/kernel.asm.o
	rm -f ./build/kernel.o
	rm -f ./build/completeKernel.o
	rm -f ./build/VGA.o
	rm -f ./build/stdio.o
	rm -f ./build/string.o
	rm -f ./build/mem.o
	rm -f ./build/IDT.o
	rm -f ./build/ports.o
	rm -f DoorsOS.img
	rm -f ./build/8259_PIC.o
	rm -f ./build/isr.o
	rm -f ./build/isr.asm.o
	rm -f ./build/irq.asm.o
	
run-Doors:
	#qemu-system-x86_64 -hda os.bin
	qemu-system-x86_64 -drive format=raw,file=DoorsOS.bin
