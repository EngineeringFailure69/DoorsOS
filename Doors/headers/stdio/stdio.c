#include "stdio.h"

void clear_screen()
{
	int i;
	for(i=0; i<MAX_COLS * MAX_ROWS; ++i)
	{
		print_character(' ', i*2);
	}
	set_cursor(get_offset(0, 0));
}

void print_character(char character, int offset)
{
	unsigned char *videomemory = (unsigned char *) VIDEO_MEM;
	videomemory[offset] = character;
	videomemory[offset+1] = WHITE_COLOR;	 
}

void print_string(const char* string)
{
	int offset = get_cursor();
	int i=0;
	while(string[i]!=0)
	{
		if(string[i] == '\n')
		{
			offset = handle_new_line(offset);
		}
		else if(string[i] == '\t')
		{
			offset += 32; 
			set_cursor(offset); //Pomeram ga za 16 karaktera;
		}
		else
		{
			print_character(string[i], offset);
			offset += 2;
		}
		i++;
	}
	set_cursor(offset);
}

void int_to_string(int number, char* str)
{
	int i = 0, j, digit;
	int is_negative = 0;
	char temp, c;
	
	//Provera da li je broj negativan i postavljanje flag-a ukoliko jeste
	if(number<0)
	{
		is_negative = 1;
		number = -number;
	}
	
	//Izdvajam cifre iz broja i svaku ubacujem kao karakter u string
	while(number!=0)
	{
		digit = number % 10;
	        number /= 10;
	        c = '0' + digit;
	        str[i++] = c;
	}
	
	if(is_negative)
	{
		str[i++] = '-';
	}
	str[i] = '\0';
	
	//Cifre se u stringu nalaze u obrnutom redosledu, pa ih treba rotirati
	for(j=0;j<i/2;j++)
	{
		temp = str[j];
		str[j] = str[i-j-1];
		str[i-j-1] = temp;
	}
}

void fprint_int(int number)
{
	int offset = get_cursor();
	char buffer[12];
	int_to_string(number, buffer);
	print_string(buffer);
	offset = handle_new_line(offset);
	set_cursor(offset);
}

void print_int(int number)
{
	char buffer[12]; //najveci i najmanji integer imaju 10 cifara u 32b velicini, jos ako dodamo - onda ima 11 i nakon toga \0 - 12
	int_to_string(number, buffer);
	print_string(buffer);
}

/*void mirror_string(char* str)
{
	int i, n;
	char temp;
	n = strlength(str);
	//Cifre se u stringu nalaze u obrnutom redosledu, pa ih treba rotirati
	for(i=0;i<n/2;i++)
	{
		temp = str[i];
		str[i] = str[n - i - 1];
		str[n - i - 1] = temp;
	}
}

void float_to_string(float number, char* str)
{
	int i = 0, digit, j = 0;
	int is_negative = 0;
	int ceoDeo = number; //Uzimam ceo deo
	int razlomljeniDeo = (int)((number - ceoDeo) * 10);
	char c;
	char str1[10]; 
	char str2[10];
	
		
	print_int(ceoDeo);
	//Provera da li je broj negativan i postavljanje flag-a ukoliko jeste
	if(number<0)
	{
		is_negative = 1;
		number = -number;
	}
	
	//Izdvajam cifre iz broja i svaku ubacujem kao karakter u string
	while(ceoDeo > 0)
	{
		digit = ceoDeo % 10;
        	ceoDeo /= 10;
        	c = '0' + digit;
        	str1[i++] = c;
	}
	str1[i++] = '\0';
	mirror_string(str1);
	i = 0;
	
	while(razlomljeniDeo > 0)
	{
	 	digit = razlomljeniDeo % 10;
        	razlomljeniDeo /= 10;
        	c = '0' + digit;
        	str2[i++] = c;
	}
	
	str2[i]='\0';
	mirror_string(str2);	
	i=0;
	
	if(is_negative)
	{
		str[i++] = '-';
	}
	
	while(str1[j] != '\0')
	{
		str[i++] = str1[j++];
	}
	str[i] = '.';
	
	while(str2[j] != '\0')
	{
		str[i++] = str2[j++];
	}
	
	str[i] = '\0';
}

void print_float(float number)
{
	char buffer[30]; // 1 za znak, 7 za mantisu, 1 za tacku, 1 za \0, 3 za eksponent, 1 za e, 1 za znak eksponenta, 3 za cifre 
	float_to_string(number, buffer);
	print_string(buffer);
}*/

void int_to_hex(uint32_t value, char* str)
{
	char hex_digits[] = "0123456789ABCDEF";
	int i;
	for(i = 0; i < 8; i++)
	{
	str[7 - i] = hex_digits[value & 0xF];
	value = value >> 4;
	}

	str[8] = '\0';  
}

void f_print_hex(uint32_t value)
{
	int offset = get_cursor();
	print_character('B', offset);
	char buffer[9], sign[] = "0x";  // 32 bita, 8 cifara + '\0'
	int_to_hex(value, buffer);  // Konvertujemo u hex
	print_string(sign);  // Formatiramo kao heksadecimalni broj
	int i = 0;
	while (i < 7 && buffer[i] == '0') {
		i++;
	}
	print_string(&buffer[i]);  // Ispisujemo konvertovani string
	offset = handle_new_line(offset);
	set_cursor(offset);
}

void print_hex(uint32_t value)
{
	char buffer[9];  // 32 bita, 8 cifara + '\0'
	int_to_hex(value, buffer);  // Konvertujemo u hex
	print_string("0x");  // Formatiramo kao heksadecimalni broj
	int i = 0;
	while (i < 7 && buffer[i] == '0') {
		i++;
	}
	print_string(&buffer[i]);  // Ispisujemo konvertovani string
	print_string(" ");
}

void print_memory_as_hex(const char* buffer, size_t size) {
	//int offset = get_cursor();
	for (size_t i = 0; i < size; i++) {
	print_hex(buffer[i]); 
	}
}

void d_printf(const char* format, ...)
{
	va_list args;
	va_start(args, format);

	while (*format != '\0') 
	{
		if (*format == '%' && *(format + 1) == 'd') //ispisvanje int vrednosti
		{
			int value = va_arg(args, int);
			print_int(value);
			format += 2; // Preskacem "%d"
		}
		else if(*format=='%' && *(format+1)=='c') //ispisivanje karaktera
		{
			char value = (char)va_arg(args, int); //char argument se uzima kao int, jer se char automatski prosiruje u 
			//velicinu int-a kada prolazi kroz funkcije kao sto je va_arg, pa nakon sto ga prihvatim nazad moram da ga 
			//kastujem ponovo u char
			print_character(value, get_cursor());
			set_cursor(get_cursor()+2);
			format+=2; 
		} 
		else if(*format=='%' && *(format+1)=='s') //ispisivanje stringova
		{
			char* value = va_arg(args, char*);
			print_string(value);
			format+=2;
		}
		else if(*format=='%' && *(format+1)=='x') //ispisivanje hex vrednosti
		{
			uint32_t value = va_arg(args, uint32_t);
			print_hex(value);
			format+=2;
		}
		else if(*format=='%' && *(format+1)=='h' && *(format+2)=='m') //ispisuje memoriju kao hex vrednosti
		{
			char* value = va_arg(args, char*);
			print_memory_as_hex(value, sizeof(value));
			format+=3;
			
		}
		/*else if(*format=='%' && *(format+1)=='f')
		{
			float value = va_arg(args, float);
			print_float(value);
			format+=2;
		}*/
		else if(*format=='\n')
		{
			int offset = get_cursor();
			offset = handle_new_line(offset);
			set_cursor(offset);
			format++;
		}
		else if(*format=='\t')
		{
			int offset = get_cursor();
			offset += 32; 
			set_cursor(offset); //Pomeram ga za 16 karaktera;
			format++;
		}
		else 
		{
		        print_character(*format, get_cursor());
		        set_cursor(get_cursor() + 2);
		        format++;
		}
	}

	va_end(args);
}
