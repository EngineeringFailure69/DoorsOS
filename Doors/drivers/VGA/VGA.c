#include "VGA.h"

unsigned char byte_in(unsigned short port)
{
//Ova funkcija se koristi za citanje podataka direktno sa ulaznog porta
	unsigned char rezultat;
	__asm__("in %%dx, %%al" : "=a" (rezultat) : "d" (port)); //port se mapira u dx registar, izvrsi se in instrukcija, i zatim se rezultat iz al prebaci u promenljivu rezultat, i nakon toga vraca promenljivu rezultat
	return rezultat;
}

void byte_out(unsigned short port, unsigned char data)
{
//Funkcija za upis podataka direktno na izlazni port
	__asm__("out %%al, %%dx" : : "a" (data), "d" (port)); //izvrsi se out instrukcija, port se mapira na dx, data na al, i posto samo pisemo podatke direktno na port, nemamo return zato sto nije potrebno
}

void set_cursor(int offset)
{
	offset /= 2;
	byte_out(VGA_CONTROL_REGISTER, CURSOR_POSITION_HIGH_BYTE_OFFSET);
	byte_out(VGA_DATA_REGISTER, (unsigned char)(offset >> 8));
	byte_out(VGA_CONTROL_REGISTER, CURSOR_POSITION_LOW_BYTE_OFFSET);
	byte_out(VGA_DATA_REGISTER, (unsigned char)(offset & 0xff));
}

int get_cursor()
{
	int offset;
	byte_out(VGA_CONTROL_REGISTER, CURSOR_POSITION_HIGH_BYTE_OFFSET);
	offset = byte_in(VGA_DATA_REGISTER) << 8;
	byte_out(VGA_CONTROL_REGISTER, CURSOR_POSITION_LOW_BYTE_OFFSET);
	offset += byte_in(VGA_DATA_REGISTER);
	return offset * 2;
}

int get_offset(int cols, int rows)
{
	return 2 * (rows * MAX_COLS + cols);
}

int get_row_from_offset(int offset)
{
	return offset / (2 * MAX_COLS);
}

int handle_new_line(int offset)
{
	return get_offset(0, get_row_from_offset(offset) + 1);
}

/*void clear_screen()
{
	int i;
	for(i=0; i<MAX_COLS * MAX_ROWS; ++i)
	{
		print_character(' ', i*2);
	}
	set_cursor(get_offset(0, 0));
}*/
