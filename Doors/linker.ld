ENTRY(_start)
OUTPUT_FORMAT(binary)
SECTIONS
{
    . = 0x1000;
    .text :
    {
        *(.text*)
    }
    
    .data :
    {
        *(.data)
    }
    
    .rodata :
    {
        *(.rodata)
    }
    
    .bss :
    {
    	_bss_start = .;
        *(COMMON)
        *(.bss)
        _bss_end = .;
    }
}
