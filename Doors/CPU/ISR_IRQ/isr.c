#include "isr.h"

void (*isr_table[])() = {isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7, isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23, isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31};

void (*irq_table[])() = {irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7, irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15};

isr_t interrupt_handlers[256];

void isr_handler(registers_t* registers)
{
    d_printf("Primljen je interrupt: %d\n", registers->interrupt_number);
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t* registers)
{
    if(interrupt_handlers[registers->interrupt_number] != 0) //proveravam da li je interrupt registrovan
    {
    	isr_t handler = interrupt_handlers[registers->interrupt_number];
    	handler(registers);
    }

    pic_send_eoi((uint8_t)registers->interrupt_number); //Saljem interrupt_number da bi se u EOI utvrdilo kome se salje signal za kraj, masteru ili i masteru i slave-u
}

void isr_install()
{
    for(int i=0; i<32; i++)
    	idt_set_entry(i, (uint32_t)isr_table[i], 0x8E);
    
    pic_disable();;
    pic_remap();
    
    for(int i=0; i<16; i++)
    	idt_set_entry(i + 32, (uint32_t)irq_table[i], 0x8E);
    	
    idt_init();
    
    irq_clear_mask(2); // cascade
    irq_set_mask(0); //maskiram tajmer, IRQ0 za sad jer nemam hendler
    irq_set_mask(1); //maskiram tastaturu, IRQ1 za sad je nemam hendler
    //irq_clear_mask(1); // tastatura
}
