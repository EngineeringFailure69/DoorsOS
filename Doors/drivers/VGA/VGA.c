#include "VGA.h"

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
