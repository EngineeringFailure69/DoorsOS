#include "8259_PIC.h"

void pic_send_eoi(uint8_t irq)
{
    if(irq >= 40) //ovde je 40, a ne 8 zato sto se IRQ-ovi 0-15 mapiraju na ISR-ove 32-47, pa onda od 40-47 ide slave 
    	byte_out(SLAVE_PIC_COMMAND_PORT, PIC_EOI); //ako je IRQ 8-15 poslat, odnosno ako je slave PIC u pitanju, saljem i njemu PIC_EOI
    byte_out(MASTER_PIC_COMMAND_PORT, PIC_EOI); //master PIC svakako dobija PIC_EOI, jer se njemu salje ako je poslat IRQ < 8, a salje mu se ako se salje i slave PIC-u
}

void pic_remap()
{
    uint8_t master_mask = byte_in(MASTER_PIC_DATA_PORT); //cuvam stare mask registre PIC-a
    io_wait();
    uint8_t slave_mask = byte_in(SLAVE_PIC_DATA_PORT); 
    io_wait();
    byte_out(MASTER_PIC_COMMAND_PORT, INITIALISE_COMMAND_CODE); //Inicijalizacija master PIC-a
    io_wait(); //Ovo je potrebno za starije masine, jer moze da se desi da PIC ne reaguje na odmah komande, pa mora malo da se saceka 
    byte_out(SLAVE_PIC_COMMAND_PORT, INITIALISE_COMMAND_CODE); //Inicijalizacija slave PIC-a
    io_wait();
    byte_out(MASTER_PIC_DATA_PORT, MASTER_PIC_OFFSET); //postavljam offset za master pic (0x20)
    io_wait();
    byte_out(SLAVE_PIC_DATA_PORT, SLAVE_PIC_OFFSET); //postavljam offset za slave pic (0x28)
    io_wait();
    byte_out(MASTER_PIC_DATA_PORT, SLAVE_PIC_IRQ2); //slave se nalazi na IRQ2 i to javljamo masteru
    io_wait();
    byte_out(SLAVE_PIC_DATA_PORT, SLAVE_PIC_CASCADE_IDENTITY); //slave dobije informaciju da se radi o cascade rezimu
    io_wait();
    byte_out(MASTER_PIC_DATA_PORT, PIC_8086_MODE); //prebacujem mod
    io_wait();
    byte_out(SLAVE_PIC_DATA_PORT, PIC_8086_MODE);
    io_wait();
    //Unmask oba PIC-a, ovo bi ukljucilo sve interrupt-ove odjednom, sto moze da bude malo manje bezbedno u ovom stadijumu
    //byte_out(MASTER_PIC_DATA_PORT, PIC_UNMASK);
    //byte_out(SLAVE_PIC_DATA_PORT, PIC_UNMASK); 
    //Vracam stare register
    byte_out(MASTER_PIC_DATA_PORT, master_mask);
    byte_out(SLAVE_PIC_DATA_PORT, slave_mask); 
}
void irq_set_mask(uint8_t irq_line)
{
    uint16_t port;
    uint8_t value;
    
    if(irq_line < 8)
        port = MASTER_PIC_DATA_PORT;
    else
    {
    	port = SLAVE_PIC_DATA_PORT;
    	irq_line -= 8; //8 se oduzima zato sto IRQ 8-15 moraju redom da postanu bitovi 0-7, jer slave PIC, kao i master, ima bitove 0-7, pa onda recimo ako se posalje IRQ12, i oduzme se 8, dobijam 12-8=4, odnosno to ce biti bit 4 na slave PIC-u (PIC mask register je 8-bitna vrednost gde svaki bit predstavlja jedan IRQ, i u zavisnosti od toga koja cifra ima koju vrednost, bit je ili maskiran (iskljucen), ili nije maskiran(ukljucen))
    }
    value = byte_in(port) | (1 << irq_line); // (1 << irq_line) deo linije pravi masku, odnosno, ako recimo uzmemo irq_line = 1, tu bi kombinacija bitova bila 00000001 (2^0 = 1), kada uradim 1 << tada imam 00000010 (shift u levo za jedno mesto), e sad kad primenim OR (|) operator, sa trenutnom vrednoscu PIC mask registra koja je npr. 00000000 (svi interrupt-ovi ukljuceni), imam 00000000 OR 00000010, odnosno dobijam 00000010 sto znaci da se IRQ1 ne ukljucuje, odnosno to je tastatura i ona se maskira jer bit = 1 znaci da je taj IRQ maskiran i nije dozvoljen da se izvrsi
    byte_out(port, value);
}
void irq_clear_mask(uint8_t irq_line)
{
    uint16_t port;
    uint8_t value;
    
    if(irq_line < 8)
    	port = MASTER_PIC_DATA_PORT;
    else
    {
	port = SLAVE_PIC_DATA_PORT;
	irq_line -= 8; //ista prica kao gore
    }
    value = byte_in(port) & ~ (1 << irq_line); //(1 << irq_line) deo linije pravi masku, odnosno, ako recimo uzmemo irq_line = 1, tu bi kombinacija bitova bila 00000001 (2^0 = 1), kada uradim 1 << tada imam 00000010 (shift u levo za jedno mesto), kada primenim ~ (NOT) operator onda invertujem sve bitove i dobijam 11111101, e sad kad primenim AND (&) operator na trenutnu vrednost PIC mask registra, koja je npr 00000010 (IRQ1-tastatura), dobijam 00000010 AND 11111101 sto je 00000000, odnosno brisem taj jedan bit i omogucujem taj IRQ za tastaturu, jer bit = 0 znaci dozvoljen
    byte_out(port, value);
}

void pic_disable(void)
{
    byte_out(MASTER_PIC_DATA_PORT, 0Xff);
    byte_out(SLAVE_PIC_DATA_PORT, 0Xff);
}
