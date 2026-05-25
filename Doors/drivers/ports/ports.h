#ifndef PORTS_H
#define PORTS_H
unsigned char byte_in(unsigned short port);
void byte_out(unsigned short port, unsigned char data);
static inline void io_wait(void) 
{
//Funkcija za cekanje (1-4 mikrosekundi), korisna je za delay tokom PIC remapping-a na starijim racunarima, ili generalno kao tajmer za malo i neprecizno cekanje. Na Linux-u se uglavnom koristi port 80, ali moze da se koristi bilo koji drugi port koji je slobodan
	byte_out(0x80, 0);
}
#endif
