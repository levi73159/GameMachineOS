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

void keyboard(Registers *regs)
{
    static bool shiftPressed = false;
    static char inputString[MAX_STRING_LENGTH] = "";
    static int stringLength = 0;
    // Read the keyboard input data from the appropriate I/O port(s)
    uint8_t data = i686_inb(KEYBOARD_DATA_PORT);
    // Check the most significant bit of the data byte
    // If it is set (1), it indicates a key release event, so we ignore it
    if (data & 0x80)
    {
        // Check if the released key is the left shift
        if (data == (SCAN_CODE_LEFT_SHIFT | 0x80))
        {
            shiftPressed = false;
        }
        // Check if the released key is the right shift
        else if (data == (SCAN_CODE_RIGHT_SHIFT | 0x80))
        {
            shiftPressed = false;
        }
        return;
    }
    // Determine if the shift key is pressed
    if (data == SCAN_CODE_LEFT_SHIFT || data == SCAN_CODE_RIGHT_SHIFT)
    {
        shiftPressed = true;
        return;
    }
    if (data == SCAN_CODE_ENTER)
    {
        // Print the accumulated string
        printf("%s\n", inputString);
        memcpy(g_CurrentInput, inputString, sizeof(inputString));
        haveInput = true;
        // Clear the string
        memset(inputString, 0, sizeof(inputString));
        stringLength = 0;
        driver->Mask(1);
        return;
    }
    // Check if the Backspace key is pressed
    if (data == SCAN_CODE_BACKSPACE)
    {
        // Delete the last character if the string is not empty
        if (stringLength > 0)
        {
            // Remove the last character from the string
            inputString[--stringLength] = '\0';
            VGA_setScreenXY(stringLength, VGA_getScreenY());
            putc('\0');
            VGA_setScreenXY(0, VGA_getScreenY());
        }
        return;
    }
    // Convert the scan code to a character, considering the shift key state
    char key = convertScanCodeToChar(data, shiftPressed);
    // Append the character to the string if it is a printable character
    if (key != '\0')
    {
        // Check if the string is already at its maximum length
        if (stringLength >= MAX_STRING_LENGTH - 1)
        {
            printf(putc, "Maximum input length reached.\n");
            return;
        }
        // Append the character to the string
        inputString[stringLength++] = key;
        inputString[stringLength] = '\0';
    }
    // Print the accumulated string
    printf("%s", inputString);
    VGA_setScreenXY(0, VGA_getScreenY());
}

void waitTillInput()
{
    while (!haveInput)
        continue;
    haveInput = false;
}

void __attribute__((section(".entry"))) start(uint16_t bootDrive)
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start));

    HAL_Init();
    log_info(__FILE__, "Hall initialized!");
    driver = i8259_GetDriver();
    log_info(__FILE__, "Kernel started");
    log_info(__FILE__, "bootDriveId=%d, driverName=%s", bootDrive, driver->Name);

    i686_IRQ_RegisterHandler(1, keyboard);

    printf("GameMachine version 1.7.1e Booted...\n\n");

    printf("Welcome to the GameMachine!\n");
    printf("Name:\n");

    driver->Unmask(1); // keyboard port
    waitTillInput();
    char name[MAX_STRING_LENGTH] = "";
    memcpy(name, g_CurrentInput, MAX_STRING_LENGTH);
    printf("Welcome %s\n", g_CurrentInput);

    while (true)
    {
        printf("%s_gamemachine%d[/]\n", name, bootDrive);
        driver->Unmask(1); // keyboard port
        waitTillInput();
        if (memcmp(g_CurrentInput, "clear", 5) == 0)
            clear();
        else if (memcmp(g_CurrentInput, "help", 4) == 0)
            printf("clear help crash_me hlt di ei\n");
        else if (memcmp(g_CurrentInput, "crash_me", 8) == 0)
            crash_me();
        else if (memcmp(g_CurrentInput, "exit", 3) == 0)
            goto end;
        else
        {
            setColor(RED);
            printf("Error: Invalid command: %s\n", g_CurrentInput);
            setColor(WHITE);
        }
    }

end:
    log_info(__FILE__, "Kernel ended\n");
    for (;;)
    {
    }
}
