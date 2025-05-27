#ifndef VGA_H
#define VGA_H

#include "types.h"

void clear_screen();
void print_char(char c, int x, int y);
void print_str(const char* str, int x, int y);
void print_num(int num, int x, int y);

#endif