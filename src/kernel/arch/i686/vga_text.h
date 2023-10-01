#pragma once

void VGA_clrscr();
void VGA_putc(char c);

void VGA_setScreenXY(int x, int y);
int VGA_getScreenX();
int VGA_getScreenY();
void VGA_setColor(uint8_t color);