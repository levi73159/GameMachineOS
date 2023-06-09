#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <hal/vfs.h>

void fputc(char c, fd_t file);
void fputs(const char *str, fd_t file);
void vfprintf(fd_t file, const char *fmt, va_list args);
void fprintf(fd_t file, const char *fmt, ...);
void fprint_buffer(fd_t file, const char *msg, const void *buffer, uint32_t count);

void putc(char c);
void puts(const char *str);
void printf(const char *fmt, ...);
void print_buffer(const char *msg, const void *buffer, uint32_t count);

void setColor(uint8_t color);
void clear();

void debugc(char c);
void debugs(const char *str);
void debugf(const char *fmt, ...);
void debug_buffer(const char *msg, const void *buffer, uint32_t count);

#define BLUE 0x01
#define GREEN 0x02
#define CYAN 0x03
#define RED 0x04
#define PURPLE 0x05
#define BROWN 0x06
#define GRAY 0x07
#define DARK_GRAY 0x08
#define LIGHT_BLUE 0x09
#define LIGHT_GREEN 0x0A
#define LIGHT_CYAN 0x0B
#define LIGHT_RED 0x0C
#define LIGHT_PURPLE 0x0D
#define YELLOW 0x0E
#define WHITE 0x0F