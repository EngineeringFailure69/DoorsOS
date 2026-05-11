#include "../string/string.h"
#include "../../drivers/drivers.h"
#include "../consts_types.h"

#define VIDEO_MEM 0xb8000
#define WHITE_COLOR 0x0f
#define NULL ((void*)0)

typedef char* va_list; //Pokazivac na stek za pristup elementima 

#define va_start(ap, last_arg) (ap = (va_list)&last_arg + sizeof(last_arg)) //Postavlja pokazivac na prvi naredni element nakon char* format u d_printf.
//ap - pokazivac, last_arg - prvi argument nakon format, odnosno poslednji format argument
// ap = (va_list)&last_arg + sizeof(last_arg) - ovde u sustini u pokazivac ap postavljam adresu last_arg i na nju dodajem velicinu last_arg kako bih se pomerio na prvi element za ispis
#define va_arg(ap, type) (*(type*)((ap += sizeof(type)) - sizeof(type))) //va_arg se koristi za hvatanje elemenata za ispis, pomera va_list za velicinu tipa koji ispisujemo tako sto kastujemo u tip koji ispisujemo, i onda pomeramo pokazivac na naredni element preko ap+=sizeof(type), a onda koristimo ap - sizeof(type) da dereferenciramo prethodnu vrednost sa steka i vratimo je
//Primer:
/*
	Stek:
	| Adresa | Vrednost |
	| 0x1000 | 42 (int) |
	| 0x1004 | 64(int)  |
-va_start ce postaviti pokazivac va_list na vrh steka, odnosno na vrednost 42 i adresu 0x1000
-va_arg ce imati zadatak da pomeri pokazivac na narednu vrednost, odnosno 64 i adresu 0x1004, i vrati vrednost sa prethodne adrese, odnosno 42 sa adrese 0x1000, i to ce uraditi na sledeci nacin:
ap+=sizeof(type) - pomera pokazivac na adresu 0x1004 na steku i priprema se za ispis narednog elementa
ap-sizeof(type) - dereferencira prethodnu vrednost sa adrese 0x1000 i vraca je za ispis 
*/	
#define va_end(ap) (ap = NULL) //Za kraj rada sa va_list, dealociranje memorije takoreci

void clear_screen();
void print_character(char character, int offset);
void print_string(const char* string);
void int_to_string(int number, char* str);
void fprint_int(int number);
void print_int(int number);
void print_float(float number);
void float_to_string(float number, char* str);
void f_print_hex(uint32_t value);
void print_hex(uint32_t value);
void int_to_hex(uint32_t value, char* str);
void print_memory_as_hex(const char* buffer, size_t size); 
void d_printf(const char* format, ...);
void mirror_string(char str[]);
float my_fabsf(float x);
