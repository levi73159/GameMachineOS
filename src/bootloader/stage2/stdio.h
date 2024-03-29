#pragma once
#include <stdint.h>

void clear();
void putc(char c);
void puts(const char *str);
void printf(const char *fmt, ...);
void print_buffer(const char *msg, const void *buffer, uint32_t count);

void debug_putc(char c);

void debug_puts(const char *str);