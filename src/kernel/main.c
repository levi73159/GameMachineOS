#include <stdint.h>
#include "stdio.h"
#include "memory.h"

extern uint8_t __bss_start;
extern uint8_t __end;

void __attribute__((section(".entry"))) start(uint16_t bootDrive)
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start));

    clrscr();
    printf("GameMachine version 1.7.0e Booted...\n\n");

    printf("Welcome to the GameMachine!\n");
    printf("printf test: \n");
    printf("Number %d, string \"%s\"\n", 1234, "hello");

    
end:
    for (;;);
}
