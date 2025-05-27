#include "vga.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define WHITE_ON_BLACK 0x0F

volatile uint16_t* vga_buffer = (uint16_t*) 0xB8000;

void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = (WHITE_ON_BLACK << 8) | ' ';
    }
}

void print_char(char c, int x, int y) {
    vga_buffer[y * VGA_WIDTH + x] = (WHITE_ON_BLACK << 8) | c;
}

void print_str(const char* str, int x, int y) {
    for (int i = 0; str[i]; i++) {
        print_char(str[i], x + i, y);
    }
}

void print_num(int num, int x, int y) {
    char buffer[16];
    int i = 0;
    if (num == 0) {
        print_char('0', x, y);
        return;
    }
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    for (int j = 0; j < i; j++) {
        print_char(buffer[i - j - 1], x + j, y);
    }
}