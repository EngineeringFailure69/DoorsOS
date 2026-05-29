#include "keyboard.h"

int buffer_index = 0; 
uint8_t keyboard_buffer[KEYBOARD_BUFFER_SIZE];

void buffer_push(uint8_t scancode)
{
    if(buffer_index < KEYBOARD_BUFFER_SIZE)
    {
	keyboard_buffer[buffer_index] = scancode;
	buffer_index++;
    }
}

uint8_t buffer_pop()
{
    if(buffer_index == 0)
    	return 0;
    uint8_t scancode = keyboard_buffer[0];
    for(int i = 0; i < buffer_index - 1; i++)
    	keyboard_buffer[i] = keyboard_buffer[i + 1];
    buffer_index--;
    return scancode;
} 

void print_keyboard_character()
{
    if(buffer_index == 0)
    	return;
    uint8_t scancode = buffer_pop();
    if(scancode & 0x80)
        return;
    if(scancode == 0x1E)
    	print_string("a");
    else if(scancode == 0x1C)
    	print_string("\n");
    else if(scancode == 0x26)
    	print_string("l");
    else if(scancode == 0x2C)
    	print_string("z");
    else if(scancode == 0x13)
    	print_string("r");
    //else if(scancode != 0x1E && scancode != 0x1C)
    	//d_printf("\nUnknown character\n");
}

static void keyboard_handler(registers_t* registers)
{
    uint8_t scancode = byte_in(KEYBOARD_PORT);
    buffer_push(scancode);
    //d_printf("%x\n", scancode);
    (void)registers;
}

void init_keyboard() {
    irq_clear_mask(1); // tastatura
    register_interrupt_handler(IRQ1, keyboard_handler);
}
