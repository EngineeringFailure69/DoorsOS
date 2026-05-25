//#ifndef 8259_PIC_H
//#define 8259_PIC_H

#include "../../headers/headers.h"
#include "../../drivers/drivers.h"

#define MASTER_PIC 0x20 //base adresa za master pic
#define SLAVE_PIC 0xA0 //base adresa za slave pic
#define MASTER_PIC_COMMAND_PORT MASTER_PIC
#define SLAVE_PIC_COMMAND_PORT SLAVE_PIC
#define MASTER_PIC_DATA_PORT (MASTER_PIC + 1)
#define SLAVE_PIC_DATA_PORT (SLAVE_PIC + 1)
#define PIC_EOI 0x20 //End of interrupt kod komande, salje se na kraju interrupt rutine, ako je interrupt dosao sa master PIC-a, informacija o kraju se salje samo njemu, ako je dosao sa slave PIC-a salje se i master-u i slave-u

#define INITIALISE_COMMAND_CODE 0x11 //prva komanda koja se daje PIC-ovima kad se udje u protected mode, ovo bi trebalo da se pise kao (ICW1_INIT | ICW1_ICW4) gde bi ICW1_INIT bila vrednost 0x10 za inicijalizaciju, a ICW1_ICW4 bi oznacavalo da je ICW4 prisutno, odnosno to su neke dodatne informacije o okruzenju, imalo bi vrednost 0x01, i onda kad se uradi OR operacija dobija se 0x11
#define MASTER_PIC_OFFSET 0x20 //Postavljamo offset za master PIC na 0x20 jer odatle krecu ne rezervisani vektori, zato sto su vektori od 0x00 do 0x1F rezervisani od strane procesora za neke CPU greske u protected modu, i koriscenje ovih vektora bi moglo da dovede do toga da ne znamo da li se radi o softverskim greskama ili je IRQ
#define SLAVE_PIC_OFFSET 0x28 //Ovo je offset za slave PIC, i on krece od 0x28 jer je 0x20 - 0x27 rezervisano za master, odnosno za IRQ 0-7, a onda krece slave IRQ 8-15 
#define SLAVE_PIC_IRQ2 0x04 //Master dobija informaciju da je slave PIC na IRQ2
#define SLAVE_PIC_CASCADE_IDENTITY 0x02 
#define PIC_8086_MODE 0x01 //PIC-ovi koriste 8086/88 mod umesto 8080
#define PIC_UNMASK 0x00

void pic_send_eoi(uint8_t irq);
void pic_remap(); //glavni zadatak ove remap funkcije je da zapravo pomeri IRQ vektore na 0x20 za master i 0x28 za slave PIC, tako da se ne sudaraju sa vec rezervisanim od strane procesora 0x00 - 0x1F, ova funkcija ne sluzi da ukljuci IRQ-ove, tome sluze naredne dve funkcije irq_set_mask i irq_clear_mask, remap samo menja interrupt vektore koje PIC šalje procesoru, a procesor zatim koristi te vektore za pristup odgovarajućim IDT entry-jima.
void irq_set_mask(uint8_t irq_line); //iskljucuje IRQ na zadatoj liniji, odnosno maskira ga tako da vise ne bude citljiv kao vazeci
void irq_clear_mask(uint8_t irq_line); //ukljucuje IRQ na zadatoj liniji tako sto mu opet menja masku
void pic_disable(void); //treba maskirati sve IRQ-ove pre obavljanja remap
//#endif
