#include <stdint.h>
#include "stdio.h"
#include "memory.h"
#include <hal/hal.h>
#include <arch/i686/irq.h>
#include <arch/i686/i8259.h>
#include <util/keyboard.h>
#include <arch/i686/io.h>
#include <stddef.h>
#include "debug.h"

extern uint8_t __bss_start;
extern uint8_t __end;
static const PICDriver *driver = NULL;

void crash_me();

#define MAX_STRING_LENGTH 80
static char g_CurrentInput[MAX_STRING_LENGTH] = {0};
static bool haveInput = false;

uint64_t time = 0;

void __attribute__((section(".entry"))) start(uint16_t bootDrive)
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start));

    HAL_Init();
    log_info(__FILE__, "Hall initialized!");
    driver = i8259_GetDriver();
    log_info(__FILE__, "Kernel started");
    log_info(__FILE__, "bootDriveId=%d, driverName=%s", bootDrive, driver->Name);

    printf("GameMachine version 1.7.1e Booted...\n\n");

    printf("Welcome to the GameMachine!\n");

  

end:
    log_info(__FILE__, "Kernel ended\n");
    for (;;)
    {
    }
}
