#include "ports.h"

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
