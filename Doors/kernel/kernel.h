#include "../drivers/drivers.h"
#include "../headers/string/string.h"
#include "../headers/mem/memory.h"
#include "../headers/mem/dynamic.h"
#include "../headers/stdio/stdio.h"

//#ifndef KERNEL_H
//#define KERNEL_H

void _start();
void welcome_screen();
void initialize_kernel();
//#endif

//int broj = 5;
	//int broj2 = -5;
	//int broj3 = 12345;
	//int broj4 = -12345;
	//uint32_t test1 = 0x12abcd;
	//int broj5;
	//char hi[] = "Hello World from C kernel, im alive :)\n";
	//char src[] = "String koji se kopira\n";
	//char first[] = "Isti suuu\n";
	//char second[] = "Isti su\n";
	//char dest[40];
	//char dest1[7];
	//char src1[] = "Hello World!";
	//size_t n = 6;
	//int n=10;
	//int *ptr1, *ptr2;
	//char* string = "Hello folks\n";
	//const char test_string[] = "Hello, World!";
	//print_string(test_string);
	//uint32_t address;
	//clear_screen();
	//print_memory_as_hex(hi, sizeof(hi));
	//print_memory_as_hex("Hello folks", sizeof("Hello folks"));
	//print_string(test_string);
	//print_string("Hello World from DoorsOS kernel :)\n");
	//print_int(5);
	//print_hex(0x123);
	//d_printf("Tesss %s%c %d cc hexa: %x", "Kura", 'c', 69, 0x12abcd);
	//print_hex(test1); // Ovo će prikazati adresu pokazivača
	//print_character('X', get_cursor());
	//broj5 = strlength("Hello");
	//sprint_string("Hi World");
	//print_hex(0x123A);
	//print_int(broj);
	//print_int(broj2);
	//print_int(1234);
	//print_int(broj4);
	//print_int(broj5);
	//broj5 = strlength(hi);
	//print_int(broj5);
	//strcopy(dest, src);
	//print_string(dest);
	//broj5 = strcompare(first, second);
	//print_int(broj5);
	//memcopy(dest1, src1, n);
	//dest1[n] = '\0';
	//print_string(dest1);
	//print_hex(test1);
	//memset(src, 0x00, sizeof(src));
	//print_memory_as_hex(src, sizeof(src));
	//print_string(src);
	//strncopy(dest1, src, 6);
	//print_string(dest1);	
	/*ptr1 = (int *) memory_allocation(n * sizeof(int));
	int offset;
    for (int i = 0; i < n; ++i) 
    {
        ptr1[i] = i + 1;
    }
    print_memory_as_hex(ptr1, n * sizeof(int));  // Ispis trenutne memorije
    offset = get_cursor();
    offset = handle_new_line(offset);
    set_cursor(offset);
    for(int i=0;i<n;i++)
    	print_int(ptr1[i]);  // Provera prvog elementa
    
    // Oslobodi memoriju
    memory_free(ptr1);
    ptr1 = NULL_POINTER;

    // Ponovo alociraj memoriju (iste ili veće veličine)
    ptr2 = (int *) memory_allocation(n * sizeof(int));
    print_memory_as_hex(ptr2, n * sizeof(int));  // Proveri da li je alociran isti blok
	offset = get_cursor();
    offset = handle_new_line(offset);
    set_cursor(offset);
    // Ako je blok isti, podaci će biti nepromenjeni
    // Popuni ponovo memoriju novim vrednostima
    for (int i = 0; i < n; i++)
    {
    	print_int(ptr2[i]);
        //ptr2[i] = (i + 1) * 2;  // Postavi nove vrednosti
    }
    print_memory_as_hex(ptr2, n * sizeof(int));  // Proveri novi sadržaj
	offset = get_cursor();
    offset = handle_new_line(offset);
    set_cursor(offset);
    // Ispis novih vrednosti
    for (int i = 0; i < n; ++i)
    {
        print_int(ptr2[i]);  // Treba da ispiše 2, 4, 6, ...
    }*/
