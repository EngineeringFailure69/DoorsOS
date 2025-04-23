[BITS 32]

;global _start

[extern _start]

;_start:
call _start
jmp $
	
;times 512 - ($ - $$) db 0 
	
