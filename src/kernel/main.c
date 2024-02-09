#include <stdint.h>
#include "stdio.h"
#include "memory.h"
#include <hal/hal.h>
#include <arch/i686/irq.h>
#include <arch/i686/i8259.h>
// #include </keyboard.h>
#include <arch/i686/io.h>
#include <arch/i686/vga_text.h>
#include <stddef.h>
#include "debug.h"

extern uint8_t __bss_start;
extern uint8_t __end;

void crash_me();
extern void _init();

uint64_t time = 0;

#define NUM(...) NUM_(__VA_ARGS__, , , , , , , , , , )
#define NUM_(...) NUM_MSVCHACK((__VA_ARGS__))
#define NUM_MSVCHACK(numlist_) NUM__ numlist_
#define NUM__(a1_, a2_, a3_, a4_, a5_, a6_, a7_, a8_, ...) a1_##a2_##a3_##a4_##a5_##a6_##a7_##a8_

void __attribute__((section(".entry"))) start(uint16_t bootDrive)
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start));
    _init(); // calling global instructers

    HAL_Init();
    clear();
    log_info(__FILE__, "Hall initialized!");
    log_info(__FILE__, "Kernel started");
    log_info(__FILE__, "bootDriveId=%d", bootDrive);


    printf("GameMachine version 1.7.1f Booted...\n\n");

    printf("Welcome to the GameMachine!\n");
    printf("Name:\n");

    const char *input = gets(SCAN_CODE_ENTER, true);
    const char name[MAX_STRING_LENGTH];
    memcpy(name, input, MAX_STRING_LENGTH);
    putc('\n');
    printf("Welcome, %s!\n", name);


    while (true)
    {
        printf("%s_gamemachine-%d[/]\n", name, bootDrive);
        gets(SCAN_CODE_ENTER, true); // we already have input array ptr
        putc('\n');
        if (memcmp(input, "clear", 5) == 0)
            clear();
        else if (memcmp(input, "help", 4) == 0)
            printf("clear help crash_me hlt di ei\n");
        else if (memcmp(input, "crash_me", 8) == 0)
            crash_me();
        else if (memcmp(input, "exit", 3) == 0)
            goto end;
        else
        {
            setColor(RED);
            printf("Error: Invalid command: %s\n", input);
            setColor(WHITE);
        }
    }

end:
    log_info(__FILE__, "Kernel ended\n");
    for (;;)
    {
    }
}

// void waitTillInput()
// {
//     while (!haveInput)
//         continue;
//     haveInput = false;
// }