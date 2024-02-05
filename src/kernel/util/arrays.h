#pragma once
#include <stdint.h>

#include <stdint.h>
#include <stdbool.h>

#define SIZE(array) (sizeof(array) / sizeof(array[0]))

uint32_t getStringLength(const char *str);
bool getFree(void *array, uint32_t size, uint32_t *outIndex);