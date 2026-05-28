#ifndef ISR_H
#define ISR_H

#include "../../headers/headers.h"
#include "../../headers/consts_types.h"
#include "../8259_PIC/8259_PIC.h"
#include "../IDT/IDT.h"

//Ovo su ISR vrednosti na koje se mapiraju IRQ-ovi 0-15
#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

//prva 32 rezervisana ISR-a za CPU, nalaze se u isr.asm fajlu koji generise labelu isr pa onda broj ISR-a koriscenjem makroa, i ostatak koda u zavisnosti od toga da li radi automatski push greske ili ne, a ja mu ovde pristupam sa extern jer je u drugom fajlu i koristim ime funkcije koju je generisao makro kao labelu u asm fajlu
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

//Ovo je tabela svih ISR-ova, odnosno niz pokazivaca na funkcije napisane odozgo
extern void (*isr_table[])(); 

//IRQ-ovi, slican princip kao za ISR-ove iznad samo sto je fajl irq.asm
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

//Tabela svih IRQ-ova, niz pokazivaca na funkcije 
extern void (*irq_table[])();

typedef struct
{
    uint32_t ds; //Data segment selector
    uint32_t edi,esi,ebp,esp,ebx,edx,ecx,eax; //ovo je sve pusha stavio na stek
    uint32_t interrupt_number, error_code; //broj interrupt-a i error code ako ga ima
    uint32_t eip, cs, eflags, useresp, ss; //ovo procesor stavlja automatski na stek
}registers_t;

typedef void (*isr_t) (registers_t*); //Omogucuje callback-ove za interrupt-ove. Za IRQ-ove koristim gore definisane vrednosti za mapiranje

void isr_handler(registers_t* registers);
void irq_handler(registers_t* registers);
void register_interrupt_handler(uint8_t n, isr_t handler);
void isr_install();

#endif
