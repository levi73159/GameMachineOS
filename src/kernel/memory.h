#pragma once
#include "stdint.h"

void *memcpy(void *dst, const void *src, uint16_t num);
void *memset(void *ptr, int value, uint16_t num);
int memcmp(const void *ptr1, const void *ptr2, uint16_t num);

#define SEG(segOff) (segOff >> 16)
#define OFF(segOff) (segOff & 0xFFFF)
#define SEGOFF2LIN(segOff) ((SEG(segOff) << 4) + OFF(segOff))