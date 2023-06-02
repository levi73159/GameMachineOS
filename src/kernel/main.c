#include <stdint.h>
#include "stdio.h"
#include "memory.h"
#include <hal/hal.h>
#include <arch/i686/irq.h>
#include <arch/i686/i8259.h>
#include <util/keyboard.h>
#include <arch/i686/io.h>
#include <stddef.h>
#include "memdefs.h"
#include <arch/i686/vbe.h>

extern uint8_t __bss_start;
extern uint8_t __end;
static const PICDriver *driver = NULL;

void crash_me();

#define MAX_STRING_LENGTH 80
static char g_CurrentInput[MAX_STRING_LENGTH] = {0};
static bool haveInput = false;
static int squareSize = 500;
static int prevSquareSize = 500;

uint64_t time = 0;

#define COLOR(r, g, b) ((b) | (g << 8) | (r << 16))
#define CORD(x, y, info) (y * info->pitch / 4 + x)

VbeModeInfo *modeInfo;
uint32_t *fb;
int w;
int h;
int squareX;
int squareY;
uint32_t squareColor; // Red color

void clearScreen(int w, int h, VbeModeInfo *minf, uint32_t color, uint32_t *fb)
{
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            fb[CORD(x, y, minf)] = color;
        }
    }
}

void timer()
{
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            // Set the entire framebuffer to white color
            fb[CORD(x, y, modeInfo)] = COLOR(255, 255, 255);

            // Draw the square with red color
            if (y >= squareY && y < squareY + squareSize && x >= squareX && x < squareX + squareSize)
            {
                fb[CORD(x, y, modeInfo)] = squareColor;
            }
        }
    }
}

void keyboard(Registers *regs)
{
    uint8_t data = i686_inb(KEYBOARD_DATA_PORT);
    prevSquareSize = squareSize;

    if (data == SCAN_CODE_EQUAL)
        squareSize++;
    else if (data == SCAN_CODE_MINUS && squareSize > 1)
        squareSize--;
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
    driver = i8259_GetDriver();

    i686_IRQ_RegisterHandler(1, keyboard);
    i686_IRQ_RegisterHandler(0, timer);
    i686_IRQ_RegisterHandler(12, mouseHandler); // Register mouse interrupt handler on IRQ 12

    modeInfo = (VbeModeInfo *)MEMORY_MODE_INFO;
    fb = (uint32_t *)(modeInfo->framebuffer);
    w = modeInfo->width;
    h = modeInfo->height;
    squareX = (w - squareSize) / 2;
    squareY = (h - squareSize) / 2;
    squareColor = COLOR(255, 0, 0); // Red color

    clear();

    driver->Unmask(1);  // keyboard port
    driver->Unmask(12); // mouse port

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
            fb[CORD(x, y, modeInfo)] = COLOR(x + 25, x * y, x + 15);
    }
end:
    for (;;)
    {
    }
}
