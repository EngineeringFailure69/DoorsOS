#include "isr.h"

//Malo objasnjenje kako radi interrupt handling sistem: isr_t interrupt_handlers[256]; //Niz pokazivaca na registrovane interrupt handlere za svih 256 IDT ulaza odnosno, to je niz pokazivaca tipa isr_t gde je svaki isrt_t pokazivac na funkciju koja prima registers_t* i vraca void. E sad, kada stigne neki interrupt, recimo da stigne IRQ1, sto je tastatura, on se registruje u kernelu, odnosno tokom inicijalizacije kernela pomocu funkcije register_interrupt_handler gde se za parametar n salje IRQ1, odnosno 33 jer su tako mapirani IRQ-ovi (mapiraju se od 32-47, a tastatura je 33 jer je IRQ0 tajmer i on ima vrednost 32), drugi parametar funkcije je isr_t handler, tu se salje pokazivac na funkciju koja sluzi za obradu tog IRQ-a, recimo na primeru tastature bi to bila funkcija koja cita sa porta x60 jer je na tom portu tastatura, i onda bi se ta funkcija poslala kao drugi parametar, odnosno pokazivac na tu funkciju. Nakon sto je na indeksu 33 u ovom slucaju, registrovana adresa te funkcije za tastaturu, i prekid je stigao, krece se sa obradom prekida, smestaju se registri, i poziva se funkcija irq_handler (irq_stub) koja prima registers, proverava da li je handler sa odredjenim interrupt brojem, u ovom slucaju 33, registrovan, ukoliko jeste, kreiram novi handler tipa isr_t i uzimam adresu funkcije koja je registrovana na indeksu 33, odnosno uzimam adresu handler funkcije za tastaturu, pa nakon toga pozivam handler, odnosno preko pokazivaca na tu funkciju koja je bila registrovana na indeksu 33, saljem joj registers i pozivam je na taj nacin

void (*isr_table[])() = {isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7, isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23, isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31}; //ovo su pokazivaci na one funkcije iz .h

void (*irq_table[])() = {irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7, irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15};

isr_t interrupt_handlers[256]; //Niz pokazivaca na registrovane interrupt handlere za svih 256 IDT ulaza.

void isr_handler(registers_t* registers)
{
    if (registers->interrupt_number < 32)
    {
	d_printf("CPU EXCEPTION: %d\n", registers->interrupt_number);
	while(1);
    }
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t* registers)
{
    if(interrupt_handlers[registers->interrupt_number] != 0) //proveravam da li je handler registrovan
    {
    	isr_t handler = interrupt_handlers[registers->interrupt_number]; //uzimam handler ako je registrovan
    	handler(registers); //pozivam handler
    }
    pic_send_eoi((uint8_t)registers->interrupt_number); //Saljem interrupt_number da bi se u EOI utvrdilo kome se salje signal za kraj, masteru ili i masteru i slave-u
}

void isr_install()
{
    for(int i=0; i<32; i++)
    	idt_set_entry(i, (uint32_t)isr_table[i], 0x8E); //punim IDT
    
    pic_disable();
    pic_remap();
    
    for(int i=0; i<16; i++)
    	idt_set_entry(i + 32, (uint32_t)irq_table[i], 0x8E);
    
    idt_init();
}
