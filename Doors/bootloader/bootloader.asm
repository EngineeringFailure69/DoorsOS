;Ovde setujemo neke osnovne segmentne registre i ukljucujemo ostale fajlove kao sto su swtich32b.asm, gdt.asm, messages.asm i ostale

[BITS 16] ; Nalazimo se u 16b real modu, u ovom modu se nalazi svaki procesor nakon pokretanja racunara i inicijalizacije hardvera od strane BIOS-a
;call print_msg1 ; Pozovi funkciju za ispis
;call delay
;call print_msg2
;call delay
;call print_msg3
;call print_msg4
[ORG 0x7c00] ; Na ovu adresu BIOS ucitava nas bootloader kada ukljucimo racunar i onda ga nakon inicijalizacije hardvera trazi na ovoj adresi

CODE_OFFSET equ 0x8
DATA_OFFSET equ 0x10

KERNEL_LOAD_SEG equ 0x1000
KERNEL_START_ADDRESS equ 0x1000

;call print_msg5
;call delay

start: 
	;call print_msg6
	;call delay
	cli ; Ovom instrukcijom kazemo procesoru da sve interrupt-ove zanemari i da nastavi sa izvrsavanjem koda, da ne bi prekinuo proces setovanja segmentnih registara
	xor ax, ax
	mov ds, ax ; Setujemo segmentni registar ds na vrednost 0 (0x00) tako sto mu dodelimo vrednost ax
	mov es, ax ; Setujemo segmentni registar es na vrednost 0 (0x00) tako sto mu dodelimo vrednost ax
	mov ss, ax ; Setujemo segmentni registar ds na vrednost 0 (0x00) tako sto mu dodelimo vrednost ax
	mov sp, 0x7c00 ; Setujemo sp (stack pointer) registar na pocetak bootloader-a, u sustini, nas stack ce rasti i pomerati pointer na dole, i postavljanjem pointera na pocetak samog bootloader-a mi cemo omoguciti da stack pravilno funkcionise i smesta kernel i ostatak sistema redom
	sti ; Ovom instrukcijom procesoru vracamo mogucnost obradjivanja interrupt-ova koje smo onemogucili pre setovanja svih segmentnih registara komdanom cli
	;cli ; Ponovo zanemarimo interrupt-ove
	;hlt ; Zaustavljamo CPU 
	
	;%include "bootloader/SwitchTo32b.asm" - dodaj posle iz zasebnog fajla
	;%include "bootloader/GDT.asm" - dodaj posle iz zasebnog fajla
	;%include "bootloader/delay.asm"
	
;Load Kernel

mov bx, KERNEL_LOAD_SEG
mov dh, 0x00 ; citamo od glave 0
mov dl, 0x80 ; citamo iz prvog hard drive-a
mov cl, 0x02 ; krecemo od drugog sektora jer se u prvom sektoru nalazi bootloader
mov ch, 0x00 ; nulti cilindar 
mov ah, 0x02 ; postavljamo na 0x02 zato sto to oznacava citanje (0x03 oznacava pisanje)
mov al, 10 ; broj sektora koje citamo
int 0x13 ; BIOS interrupt (funkcija) za pristup disku 

jc disk_read_error
	
;Call 
;call print_msg7
;call delay
;call print_msg8
;call delay	
;call print_msg9
;call delay
;call print_msg10
;call delay
;%include "bootloader/delay.asm"
switch_32b:
	cli ; iskljucujemo interrupt
	lgdt[gdt_descriptor] ; ucitavamo GDT koriscenjem descriptor-a
	mov eax, cr0 ; ucitavamo vrednost registra cr0 u eax
	or al, 1 ; postavljamo vrednost al dela registra na 1
	mov cr0, eax ; ubacujemo jedinicu u registar cr0 jer na taj nacin ukljucujemo PM (Protected Mode) bit i postavljamo ga na 1 da bi omogucili prelazak u 32b protected mode
	jmp CODE_OFFSET:init_32b ; daleki skok za prelazak na novi kodni segment definisan u GDT-u

disk_read_error:
	hlt ; u slucaju greske prekidamo procesor 

;GDT setup
;Call
;%include "bootloader/print.asm"
gdt_start:
	dd 0x00000000 ; NULL desrciptor - neophodan za ulaz u GDT
	dd 0x00000000 ; Nastavak NULL descriptor-a
		
;Code segment descriptor
gdt_code:
	dw 0xFFFF ; Segment limit (4KB granularnost, maksimalni limit)
	dw 0x0000 ; Base address (nizih 16 bitova)
	db 0x00 ; Base address (sledecih 8 bitova)
	db 10011010b ; Access byte
	db 11001111b ; Flags
	db 0x00 ; Base address (visih 8 bitova)
	
;Data segment descriptor
gdt_data:
	dw 0xFFFF ; Segment limit (isto kao code segment)
	dw 0x0000 ; Base
	db 0x00 ; Base
	db 10010010b ; Access byte
	db 11001111b ; Flags
	db 0x00 ; Base address
	
gdt_end:
	
gdt_descriptor:
	dw gdt_end - gdt_start - 1 ; Velicina GDT - 1
	dd gdt_start ; Adresa pocetka GDT-a
	
[BITS 32]
init_32b:
	mov ax, DATA_OFFSET ; Ucitaj DATA_OFFSET u ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov ss, ax
	mov gs, ax
	mov ebp, 0x9C00 ; postavi base pointer, zbog prelaska u 32b rezim
	mov esp, ebp ; postavi stack pointer (ponovo zbog prelaska u 32b rezim), i inizijalizuj stack 
	
	;call Begin_32b
	
	in al, 0x92 ; citamo vrednost iz I/O porta 0x92 (system control port)
	or al, 2 ; postavljamo bit 1 u al da bi omogucili A20 liniju (neophodno je za pristup memoriji vecoj od 1MB)
	out 0x92, al ; postavljamo vrednost iz al nazad na I/O port 0x92
	
	jmp CODE_OFFSET:KERNEL_START_ADDRESS ; skacemo na startnu adresu kernela (0x100000)
	
times 510 - ($ - $$) db 0 ; Punimo ostatak boot sektora nulama sve do 510 bajta. Objasnjenje: bootloader se nalazi u prvom sektoru diska, jedan sektor je velicine 512 bajtova i i bootloader mora da zauzme ceo prvi sektor da bi kernel moga ispravno da se ucita u narednom setoru. Potrebno je popuniti sektor do 510-og bajta nulama za slucaj da nas bootloader kod nije dovoljno veliki (kao sto nije). 511 i 512-ti bajt su rezervisani za specijalni karakter

dw 0xAA55 ; Specijalni karakter na kraju boot sektora koji BIOS trazi kao oznaku, zauzima poslednja dva bajta sektora
		
