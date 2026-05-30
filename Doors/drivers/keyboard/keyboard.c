#include "keyboard.h"

volatile int buffer_index = 0; 
uint8_t keyboard_buffer[KEYBOARD_BUFFER_SIZE];
bool numlck_pressed = false, shift_pressed = false, caps_pressed = false;

char keyboard_map[] = {[0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4', [0x06] = '5', [0x07] = '6', [0x08] = '7', [0x09] = '8', [0x0A] = '9', [0x0B] = '0', [0x0C] = '-', [0x4A] = '-' /*numeric keyboard*/, [0x0D] = '=', [0x10] = 'q', [0x11] = 'w', [0x12] = 'e', [0x13] = 'r', [0x14] = 't', [0x15] = 'y',  [0x16] = 'u', [0x17] = 'i', [0x18] = 'o', [0x19] = 'p', [0x1A] = '[', [0x1B] = ']', [0x1E] = 'a', [0X1F] = 's', [0x20] = 'd', [0x21] = 'f', [0x22] = 'g', [0x23] = 'h', [0x24] = 'j', [0x25] = 'k', [0x26] = 'l', [0x27] = ';', [0x28] = '\'', [0x29] = '`', [0x2B] = '\\', [0x2C] = 'z', [0x2D] = 'x', [0x2E] = 'c', [0x2F] = 'v', [0x30] = 'b', [0x31] = 'n', [0x32] = 'm', [0x33] = ',', [0x34] = '.', [0x35] = '/', [0x4E] = '+', [0x56] = '<'};

char keyboard_map_numlock[] = { [0x52] = '0', [0x4F] = '1', [0x50] = '2', [0x51] = '3', [0x4B] = '4', [0x4C] = '5', [0x4D] = '6', [0x47] = '7', [0x48] = '8', [0x49] = '9', [0x53] = '.'};

char keyboard_map_shift[] = {[0x02] = '!', [0x03] = '@', [0x04] = '#', [0x05] = '$', [0x06] = '%', [0x07] = '^', [0x08] = '&', [0x09] = '*', [0x0A] = '(', [0x0B] = ')', [0x0C] = '_', [0x0D] = '+', [0x10] = 'Q', [0x11] = 'W', [0x12] = 'E', [0x13] = 'R', [0x14] = 'T', [0x15] = 'Y',  [0x16] = 'U', [0x17] = 'I', [0x18] = 'O', [0x19] = 'P', [0x1A] = '{', [0x1B] = '}', [0x1E] = 'A', [0X1F] = 'S', [0x20] = 'D', [0x21] = 'F', [0x22] = 'G', [0x23] = 'H', [0x24] = 'J', [0x25] = 'K', [0x26] = 'L', [0x27] = ':', [0x28] = '\"', [0x29] = '~', [0x2B] = '|', [0x2C] = 'Z', [0x2D] = 'X', [0x2E] = 'C', [0x2F] = 'V', [0x30] = 'B', [0x31] = 'N', [0x32] = 'M', [0x33] = '<', [0x34] = '>', [0x35] = '?', [0x4E] = '+', [0x56] = '>'};

char keyboard_map_caps[] = {[0x10] = 'Q', [0x11] = 'W', [0x12] = 'E', [0x13] = 'R', [0x14] = 'T', [0x15] = 'Y',  [0x16] = 'U', [0x17] = 'I', [0x18] = 'O', [0x19] = 'P', [0x1E] = 'A', [0X1F] = 'S', [0x20] = 'D', [0x21] = 'F', [0x22] = 'G', [0x23] = 'H', [0x24] = 'J', [0x25] = 'K', [0x26] = 'L', [0x2C] = 'Z', [0x2D] = 'X', [0x2E] = 'C', [0x2F] = 'V', [0x30] = 'B', [0x31] = 'N', [0x32] = 'M', [0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4', [0x06] = '5', [0x07] = '6', [0x08] = '7', [0x09] = '8', [0x0A] = '9', [0x0B] = '0', [0x0C] = '-', [0x4A] = '-' /*numeric keyboard*/, [0x0D] = '=', [0x1A] = '[', [0x1B] = ']', [0x27] = ';', [0x28] = '\'', [0x29] = '`', [0x2B] = '\\', [0x33] = ',', [0x34] = '.', [0x35] = '/', [0x4E] = '+', [0x56] = '<'}; 

bool buffer_has_data()
{
    if(buffer_index > 0)
    	return true;
    else
    	return false;
}

void buffer_push(uint8_t scancode)
{
    if(buffer_index < KEYBOARD_BUFFER_SIZE) //provera da li je buffer pun, ukoliko jeste pun jednostavno ignorisem nove unose sa tastature za sada
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
    char keyboard_key = keyboard_map[scancode]; //iz keyboard_map uzimam karakter koji ima scancode vrednost
    if(scancode & 0x80) //ovako se zanemaruje key release kod
    {
    	if(scancode == 0xAA || scancode == 0xB6) //kada pustimo levi ili desni shift
    	    shift_pressed = false;
        return;
    }
    else if(scancode == 0x2A || scancode == 0x36) //levi i desni shift, provera da li je pritisnut 
    {
    	shift_pressed = true;
    	return;
    }
    if(scancode == 0x45) //kada se pritisne num lock menja se trenutna vrednost flag-a
    {
    	numlck_pressed = !numlck_pressed;
    	return;
    }
    else if(scancode == 0x3A) // kada se pritisne caps menja se trenutna vrednost flag-a
    {
    	caps_pressed = !caps_pressed;
    }
    else if(shift_pressed) //ukoliko je shift pritisnut
    {
    	keyboard_key = keyboard_map_shift[scancode];
    	print_character(keyboard_key, get_cursor());
    	set_cursor(get_cursor() + 2);
    }
    else if(numlck_pressed == true && !keyboard_map[scancode]) //ukljucen num lock i kuca se na numerickoj
    {
    	keyboard_key = keyboard_map_numlock[scancode];
    	print_character(keyboard_key, get_cursor());
    	set_cursor(get_cursor() + 2);
    }
    else if(caps_pressed) //ukoliko je ukljucen caps lock
    {
    	keyboard_key = keyboard_map_caps[scancode];
    	print_character(keyboard_key, get_cursor());
    	set_cursor(get_cursor() + 2);
    }
    else if(keyboard_key) //bilo koje dugme na tastaturi
    {
    	print_character(keyboard_key, get_cursor());
    	set_cursor(get_cursor() + 2);
    }
    if(scancode == 0x1C) //enter dugme prelazi u novi red
    	print_string("\n");
    else if(scancode == 0x0E) //bakspace dugme, pomeram kursor unazad za jedan karakter i na to mesto pisem prazan karakter da bih obrisao trenutni
    {
    	set_cursor(get_cursor() - 2);
    	print_character(' ', get_cursor());
    }
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
