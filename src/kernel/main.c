#include <stdint.h>
#include "stdio.h"
#include "memory.h"
#include <hal/hal.h>
#include <arch/i686/irq.h>
#include <arch/i686/i8259.h>
#include <util/keyboard.h>
#include <arch/i686/io.h>
#include <stddef.h>

extern uint8_t __bss_start;
extern uint8_t __end;
const PICDriver *driver = NULL;

void crash_me();

#define MAX_STRING_LENGTH 80
char name[MAX_STRING_LENGTH];
int time = 0;

void timer(Registers *regs)
{
    time++;

    if (time % 15 == 0)
    {
        printf("Hello %s\n", name);
    }
}

void OnInput(char input[MAX_STRING_LENGTH])
{
    printf("Hello %s\n", input);
    memcpy(name, input, MAX_STRING_LENGTH); // Copy the input string to the name variable
    name[MAX_STRING_LENGTH - 1] = '\0';     // Ensure the name string is null-terminated

    driver->Mask(1);
    driver->Unmask(0);
}

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
        OnInput(inputString);

        // Clear the string
        memset(inputString, 0, sizeof(inputString));
        stringLength = 0;
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
            setScreenXY(stringLength, getScreenY());
            putc('\0');
            setScreenXY(0, getScreenY());
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
            printf("Maximum input length reached.\n");
            return;
        }

        // Append the character to the string
        inputString[stringLength++] = key;
        inputString[stringLength] = '\0';
    }

    // Print the accumulated string

    printf("%s", inputString);
    setScreenXY(0, getScreenY());
}

void __attribute__((section(".entry"))) start(uint16_t bootDrive)
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start));

    HAL_Init();
    driver = i8259_GetDriver();
    // driver->Unmask(1);

    i686_IRQ_RegisterHandler(0, timer);
    i686_IRQ_RegisterHandler(1, keyboard);

    clear();
    setColor(GREEN);
    printf("GameMachine version 1.7.1e Booted...\n\n");
    resetColor();

    printf("Welcome to the GameMachine!\n");
    printf("Name:\n");
end:
    for (;;)
        ;
}
