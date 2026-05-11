#define VGA_CONTROL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define CURSOR_POSITION_LOW_BYTE_OFFSET 0x0f
#define CURSOR_POSITION_HIGH_BYTE_OFFSET 0x0e
#define MAX_ROWS 25
#define MAX_COLS 80

unsigned char byte_in(unsigned short port);
void byte_out(unsigned short port, unsigned char data);
unsigned short word_in(unsigned short port);
void word_out(unsigned short port, unsigned short data);
void set_cursor(int offset);
int get_cursor();
int get_offset(int cols, int rows);
int get_row_from_offset(int offset);
int handle_new_line(int offset);
