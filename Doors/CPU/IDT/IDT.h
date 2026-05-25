#include "../../headers/headers.h"

//32-bit IDT entry
typedef struct
{
    uint16_t isr_offset_low; // nizih 16 bitova isr-ove adrese (0-15)
    uint16_t kernel_cs; // Kernel segment selektor, ovo je GDT segment selektor koji CPU ucitava u CS (code segment registar) pre zvanja ISR
    uint8_t reserved; //Ovo je uvek postavljeno na 0 jer se ne koristi
    uint8_t type_attributes; //gate_type, dpl i p - gate_type se postavlja na 1110b odnosno 0xE vrednost jer koristimo 32b interrupt gate, dpl se postavlja na 00 i oznacava CPU level privilegije potrebne za pozivanje ISR-a, p se postavlja na 1 i predstavlja indikator koji kaze da li je gate aktivan
    uint16_t isr_offset_high; //visih 16 bitova isr-ove adrese (16-31)
}__attribute__((packed)) idt_entry_t;

//IDTR - ovo je lokacija IDT-a, jer se cuva u IDT registru, i ovo je u sustini pokazivac na niz interrupt handler-a
typedef struct
{
    uint16_t limit; 
    uint32_t base;
}__attribute__((packed)) idt_registry_t;

#define KERNEL_CS 0x08 // ovo je vrednost za kernel_cs iz idt_entry_t
#define NUMBER_OF_IDT_ENTRIES 256 //IDT ima 256 ulaza koji moraju biti definisani da bi se izbegla greska, moze da ima i vise ali se ignorisu

static idt_entry_t idt[NUMBER_OF_IDT_ENTRIES]; // kreiram IDT
static idt_registry_t idtr; // registar

void idt_set_entry(uint8_t vector, uint32_t isr, uint8_t flags);
void idt_init();
