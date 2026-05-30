#include "keyboard.h"

volatile int buffer_index = 0; 
uint8_t keyboard_buffer[KEYBOARD_BUFFER_SIZE];
bool numlck_pressed = false;
char keyboard_map[] = {[0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4', [0x06] = '5', [0x07] = '6', [0x08] = '7', [0x09] = '8', [0x0A] = '9', [0x0B] = '0', [0x0C] = '-', [0x4A] = '-' /*numeric keyboard*/, [0x0D] = '=', [0x10] = 'q', [0x11] = 'w', [0x12] = 'e', [0x13] = 'r', [0x14] = 't', [0x15] = 'y',  [0x16] = 'u', [0x17] = 'i', [0x18] = 'o', [0x19] = 'p', [0x1A] = '[', [0x1B] = ']', [0x1E] = 'a', [0X1F] = 's', [0x20] = 'd', [0x21] = 'f', [0x22] = 'g', [0x23] = 'h', [0x24] = 'j', [0x25] = 'k', [0x26] = 'l', [0x27] = ';', [0x28] = '\'', [0x29] = '`', [0x2B] = '\\', [0x2C] = 'z', [0x2D] = 'x', [0x2E] = 'c', [0x2F] = 'v', [0x30] = 'b', [0x31] = 'n', [0x32] = 'm', [0x33] = ',', [0x34] = '.', [0x35] = '/', [0x4E] = '+'};
char keyboard_map_numlock[] = { [0x52] = '0', [0x4F] = '1', [0x50] = '2', [0x51] = '3', [0x4B] = '4', [0x4C] = '5', [0x4D] = '6', [0x47] = '7', [0x48] = '8', [0x49] = '9', [0x53] = '.'};

bool buffer_has_data()
{
    if(buffer_index > 0)
    	return true;
    else
    	return false;
}

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
    char keyboard_key = keyboard_map[scancode];
    if(scancode & 0x80)
        return;
    if(scancode == 0x45)
    {
    	numlck_pressed = !numlck_pressed;
    	return;
    }
    if(numlck_pressed == true && !keyboard_map[scancode])
    {
    	keyboard_key = keyboard_map_numlock[scancode];
    	print_character(keyboard_key, get_cursor());
    	set_cursor(get_cursor() + 2);
    }
    else if(keyboard_key)
    {
    	print_character(keyboard_key, get_cursor());
    	set_cursor(get_cursor() + 2);
    }
    if(scancode == 0x1C)
    	print_string("\n");
    else if(scancode == 0x0E)
    	set_cursor(get_cursor() - 2);
    else if(scancode == 0x0F)
    	set_cursor(get_cursor() + 8); //pomeram kursor za 4 karaktera
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
