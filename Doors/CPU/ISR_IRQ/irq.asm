%macro IRQ 2 ;IRQ makro ima dva parametra, 1. IRQ broj (0-15), 2. ISR broj na koji je mapiran (32-47, jer su 0-31 rezervisani od strane CPU)
    global irq%1 ;stavljamo irq i prvi parametar kao global da bi mogli da ga koristimo u C-u
    irq%1:
    	cli
    	push byte 0
    	push byte %2
    	jmp irq_common_stub
%endmacro

[EXTERN irq_handler] ;ovo je funkcija u C-u, isr.c fajl

;Stub handler funkcije
IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

;irq common stub se ponasa malo drugacije od isr-a, pre nego sto se vratimo iz handler funkcije, moramo da obavestimo PIC (master i slave) da smo zavrsili obradu da bi moglo da se predje na sledeci interrupt (ukoliko postoji u redu cekanja), i ovo je poznato kao EOI (end of interrupt), naravno ukoliko je master PIC poslao interrupt (0-7) onda samo njemu saljemo EOI, ukoliko je slave poslao (8-15) onda saljemo i master i slave PIC-u (povezani su preko IRQ2 linije - daisy chaining). Irq common stub takodje cuva stanje procesora pre prekida i postavlja sve za obradu prekida, pa nakon toga vraca procesor na stanje pre prekida

irq_common_stub:

    ;cuvam kontekst procesora pre obrade

    pusha ;stavljam registre na stek u redosledu edi,esi,ebp,esp,ebx,edx,ecx,eax gde edi ide na dno steka
    xor eax, eax ;cistim eax registar
    mov ax, ds ;nizih 16 bitova registra eax = ds
    push eax ;cuvam staru vrednost ds (data segment) deskriptora
    
    ;postavljam sve sto je neophodno za obradu
    
    mov ax, 0x10 ;ucitavam kernel data segment deskriptor u ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    call irq_handler ;funkcija napisana u C-u 
    add esp, 4 ;sada esp pokazuje na registers_t strukturu iz isr.h fajla, odnosno na argument irq_handler funkcije
    
    ;vracam stari kontekst procesora da bi procesor nastavio sa izvrsenjem tamo gde je stao
    
    pop eax ;sa steka skidam stari ds (data segment deskriptor) registar koji sam sacuvao u ax
    mov ds, ax ;vracam nazad staru vrednost ds-a (nizih 16 bitova registra eax nakon skidanja sa steka)
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    popa ;skidam sve registre koje sam stavio na stek sa pusha u redosledu eax, ecx, edx, ebx, esp, ebp, esi, edi
    add esp, 8 
    iret
