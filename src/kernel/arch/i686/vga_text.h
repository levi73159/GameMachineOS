#pragma once

void VGA_clrscr();
void VGA_putc(char c);
void VGA_putcolor(int x, int y, uint8_t color);

void VGA_setScreenXY(int x, int y);
int VGA_getScreenX();
int VGA_getScreenY();
void VGA_setColor(uint8_t color);
uint8_t VGA_getColor();
void VGA_setcursor(int x, int y);