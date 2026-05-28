#include"IDT.h"

idt_entry_t idt[NUMBER_OF_IDT_ENTRIES]; // kreiram IDT
idt_registry_t idtr; // registar

void idt_set_entry(uint8_t vector, uint32_t isr, uint8_t flags)
{
    idt_entry_t* descriptor = &idt[vector];
    
    descriptor->isr_offset_low = isr & 0xFFFF; // uzimam nizih 16 bitova za offset_low pomocu AND operatora
    descriptor->kernel_cs = KERNEL_CS;
    descriptor->reserved = 0;
    descriptor->type_attributes = flags;
    descriptor->isr_offset_high = isr >> 16;
}

void idt_init()
{
    idtr.base = (uint32_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * NUMBER_OF_IDT_ENTRIES - 1; 
    
    memset(idt, 0, sizeof(idt));
    __asm__ volatile ("lidt %0" : : "m"(idtr)); //Loadovanje IDT-a
}
