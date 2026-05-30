#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../ports/ports.h"
#include "../../CPU/ISR_IRQ/isr.h"
#include "../VGA/VGA.h"
#include "../../headers/headers.h"
#include "../../headers/consts_types.h"

#define KEYBOARD_PORT 0x60
#define KEYBOARD_BUFFER_SIZE 256

bool buffer_has_data();
void init_keyboard();
void buffer_push(uint8_t scancode);
uint8_t buffer_pop();
void print_keyboard_character();

#endif
