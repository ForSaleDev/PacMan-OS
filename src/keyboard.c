#include "keyboard.h"
#include "types.h"

static char scancode_to_char(uint8_t scancode) {
    switch (scancode) {
        case 0x11: return 'w';
        case 0x1F: return 's';
        case 0x1E: return 'a';
        case 0x20: return 'd';
        default: return 0;
    }
}

char get_key() {
    uint8_t scancode = 0;
    while ((inb(0x64) & 1) == 0);
    scancode = inb(0x60);
    if (scancode & 0x80) return 0;
    return scancode_to_char(scancode);
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void init_keyboard() {
    // Nothing for now
}