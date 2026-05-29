#include "kernel.h"

void welcome_screen()
{
	clear_screen();
	print_string("Welcome to the DoorsOS, FOSS operating system for your computer :)\n");
	print_string("\n\t==================================\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t               O|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t==================================\n\nTo continue press 'Enter'");
}
void _start()
{
	//d_printf("Test float: %f\n", 9.9);
	//initialize_kernel();
	//welcome_screen();
	clear_screen();
	d_printf("Installing interrupt service routines (ISRs) and enabling external interrupts\n");
	isr_install();
	clear_screen();
	welcome_screen();
	//clear_screen();
	asm volatile ("sti"); //omoguci eksterne interrupt-ove
	//init_keyboard();
	while(1);
}
