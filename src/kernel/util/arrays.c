#include "arrays.h"

uint32_t getStringLength(const char *str)
{
    uint32_t length = 0;
    while (str[length] != '\0')
    {
        length++;
    }
    return length;
}

bool getFree(void* array, uint32_t size, uint32_t* outIndex)
{
    uint8_t *ptr = (uint8_t *)array;
    for (uint32_t i = 0; i < size; i++)
    {
        if (ptr[i] != 0)
            continue;
        *outIndex = i;
        return true;
    }
    return false;
}