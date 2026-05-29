%macro ISR_NO_ERROR_CODE 1 ;definisem makro koji prima 1 parametar, ovo je za ISR koji nemaju error kod
    [global isr%1] ;%1 se koristi za pristup tom jednom parametru, i mora da se koristi GLOBAL da bi kasnije mogao da ovaj asemblerski kod pozivam u C-u
    isr%1:
	cli ;zanemarujem sve ostale interrupt-ove
	push byte 0 ;ovo je "dummy" error kod, jer neki ISR-ovi automatski na stek stavljaju error kod, dok neki to ne rade, a da bi mogli da pozivamo hendler za sve ISR-ove podjednako, mora i stek da bude isti za svaki ISR, pa je neophodno da se na njega stavi ovaj "dummy" kod
	push byte %1 ;ovde na stek stavljam broj interrupt-a 
	jmp isr_common_stub ;skacem na common stub koji ce da pozove handler funkciju i da pripremi procesor 
%endmacro

%macro ISR_ERROR_CODE 1 ;makro koji prima 1 parametar, ovo su ISR koji automatski stavljaju error kod na stek
    [GLOBAL isr%1]
    isr%1:
    	cli
    	push byte %1 ;ovde nema "dummy" koda jer ISR automatski stavlja svoj na stek, interrupt-ovi 8, 10-14 to rade automatski
    	jmp isr_common_stub
%endmacro 
    
;Stub handler funkcije
ISR_NO_ERROR_CODE 0
ISR_NO_ERROR_CODE 1
ISR_NO_ERROR_CODE 2
ISR_NO_ERROR_CODE 3
ISR_NO_ERROR_CODE 4
ISR_NO_ERROR_CODE 5
ISR_NO_ERROR_CODE 6
ISR_NO_ERROR_CODE 7
ISR_ERROR_CODE 8
ISR_NO_ERROR_CODE 9
ISR_ERROR_CODE 10
ISR_ERROR_CODE 11
ISR_ERROR_CODE 12
ISR_ERROR_CODE 13
ISR_ERROR_CODE 14
ISR_NO_ERROR_CODE 15
ISR_NO_ERROR_CODE 16
ISR_ERROR_CODE 17
ISR_NO_ERROR_CODE 18
ISR_NO_ERROR_CODE 19
ISR_NO_ERROR_CODE 20
ISR_NO_ERROR_CODE 21
ISR_NO_ERROR_CODE 22
ISR_NO_ERROR_CODE 23
ISR_NO_ERROR_CODE 24
ISR_NO_ERROR_CODE 25
ISR_NO_ERROR_CODE 26
ISR_NO_ERROR_CODE 27
ISR_NO_ERROR_CODE 28
ISR_NO_ERROR_CODE 29
ISR_NO_ERROR_CODE 30
ISR_NO_ERROR_CODE 31

[extern isr_handler] ;ovo je funkcija u C-u, isr.c fajl

;Ovo je isr common stub koji sluzi da pre nego sto krene sa obradom IRQ-a, sacuva stanje procesora na stek, postavi sve neophodno za kernel mode segmente i obradu, odnosno da postavi sve segment registre, pozove funkciju u C-u koja se koristi za obradu, i da na kraju vrati stek i procesor u stanje pre obrade

isr_common_stub:

    ;cuvam kontekst procesora pre obrade
    
    pusha ;stavljam registre na stek u redosledu edi,esi,ebp,esp,ebx,edx,ecx,eax 
    xor eax, eax ;cistim eax registar
    mov ax, ds ;nizih 16 bitova registra eax = ds
    push eax ;cuvam staru vrednost ds (data segment) deskriptora
    
    ;postavljam sve sto je neophodno za obradu
    
    mov ax, 0x10 ;ucitavam kernel data segment deskriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    push esp
    call isr_handler ;pozivam handler napisan u C-u
    pop eax
    
    ;vracam stari kontekst procesora da bi procesor nastavio sa izvrsenjem tamo gde je stao
    
    pop eax ;sa steka skidam stari ds (data segment deskriptor) registar koji sam sacuvao u ax
    mov ds, ax ;vracam nazad staru vrednost ds-a (nizih 16 bitova registra eax nakon skidanja sa steka)
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    popa ;skidam sve registre koje sam stavio na stek sa pusha u redosledu eax, ecx, edx, ebx, esp, ebp, esi, edi
    add esp, 8 ;pomeram esp pokazivac za 8 bajtova, da bih izbrisao 4 bajta koja su stavljena na stek za broj ISR-a i za error kod
    sti
    iret ;popuje 5 registara odjednom koje je procesor automatski stavio na stek kada je krenuo u obradu: CS (code segment), EIP (instruction pointer), EFLAGS (flags registar), SS (stack segment) i ESP (stack pointer)
