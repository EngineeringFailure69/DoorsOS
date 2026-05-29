#include "keyboard.h"

void init_keyboard() {
    irq_clear_mask(1); // tastatura
}
