#include "kernel.h"

void welcome_screen()
{
	clear_screen();
	print_string("Welcome to the DoorsOS, FOSS operating system for your computer :)\n");
	print_string("\n\t==================================\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t               O|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t|\t\t|\n\t==================================\n\nTo continue press 'Enter'");
}

void initialize_kernel()
{
    d_printf("Initializing kernel:\n");
    d_printf("Installing interrupt service routines (ISRs) and enabling external interrupts\n");
    isr_install();
    d_printf("Initializing keyboard\n");
    init_keyboard();
    d_printf("Enabling external interrupts\n");
    asm volatile ("sti"); //omoguci eksterne interrupt-ove
}

void _start()
{
        bool first_screen = true;
        uint8_t scancode;
	initialize_kernel();
	clear_screen();
	welcome_screen();
	while(1)
	{
   	    if (first_screen == true)
   	    {
   	    	scancode = buffer_pop();
   	    	if(scancode == 0x1C)
   	    	{
   	    	    clear_screen();
   	    	    first_screen = false;
   	    	}
   	    }
   	    else if(!first_screen)
   	    	print_keyboard_character();
	}
}
