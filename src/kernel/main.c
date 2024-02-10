#include <stdint.h>
#include "stdio.h"
#include "string.h"
#include <hal/hal.h>
#include <arch/i686/irq.h>
#include <arch/i686/i8259.h>
// #include </keyboard.h>
#include <arch/i686/io.h>
#include <arch/i686/vga_text.h>
#include <stddef.h>
#include "memory.h"
#include "debug.h"

extern uint8_t __bss_start;
extern uint8_t __end;

void crash_me();
extern void _init();

uint64_t time = 0;
uint32_t x, y;

#define NUM(...) NUM_(__VA_ARGS__, , , , , , , , , , )
#define NUM_(...) NUM_MSVCHACK((__VA_ARGS__))
#define NUM_MSVCHACK(numlist_) NUM__ numlist_
#define NUM__(a1_, a2_, a3_, a4_, a5_, a6_, a7_, a8_, ...) a1_##a2_##a3_##a4_##a5_##a6_##a7_##a8_

char bitmap[25][80];
uint8_t drawColor = WHITE;
uint8_t background = BLACK;
bool g_shouldDraw = false;
bool shouldClear = false;

void clearBitmap()
{
    for (size_t _y = 0; _y < 25; _y++)
    {
        for (size_t _x = 0; _x < 80; _x++)
        {
            bitmap[_y][_x] = -1;
        }
    }
}

void clearArea(int x, int y)
{
    if (x < 0 || y < 0) return;
    if (x >= 80 || y >= 25) return;

    char color = bitmap[y][x];
    if (color == -1)
        VGA_putcolor(x, y, background);
    else
        VGA_putcolor(x, y, color << 4);
}

void clearAll()
{
    clear();
    for (size_t _y = 0; _y < 25; _y++)
    {
        for (size_t _x = 0; _x < 80; _x++)
        {
            char color = bitmap[_y][_x];
            if (color == -1)
                continue;
            VGA_putcolor(_x, _y, color << 4);
        }
    }
    VGA_putcolor(x, y, drawColor << 4);
    VGA_setcursor(x, y);
}

void update()
{
    if (g_shouldDraw)
    {
        bitmap[y][x] = drawColor;
        VGA_putcolor(x, y, drawColor << 4);
    }
    
    VGA_putcolor(x, y, drawColor << 4);
    VGA_setcursor(x, y);
}


void floodFill(int x, int y, char targetColor, char replacementColor)
{
    if (x < 0 || x >= 80 || y < 0 || y >= 25 || bitmap[y][x] != targetColor)
        return;

    bitmap[y][x] = replacementColor;
    floodFill(x + 1, y, targetColor, replacementColor);
    floodFill(x - 1, y, targetColor, replacementColor);
    floodFill(x, y + 1, targetColor, replacementColor);
    floodFill(x, y - 1, targetColor, replacementColor);
}

bool onKeyPress(uint8_t keyCode, Registers *regs)
{
    bool isRealised = Keyboard_Extract(&keyCode);
    if (shouldClear)
    {
        if (keyCode == SCAN_CODE_Y)
        {
            clearBitmap();
            VGA_setColor(background);
            clearAll();
            update();
            shouldClear = false;
            return true;
        } else if (keyCode == SCAN_CODE_N)
        {
            VGA_setColor(background);
            clearAll();
            update();
            shouldClear = false;
            return true;
        }
        return false;
    }
    if (isRealised)
    {
        if (keyCode == SCAN_CODE_SPACE)
        {
            g_shouldDraw = false;
            update();
        }
        return true;
    }
    switch (keyCode)
    {
    case SCAN_CODE_W:
        if (y <= 0)
            break;
        clearArea(x, y);
        y -= 1;
        break;

    case SCAN_CODE_S:
        if (y >= VGA_GetH() - 1)
            break;
        clearArea(x, y);
        y += 1;
        break;

    case SCAN_CODE_A:
        if (x <= 0)
            break;
        clearArea(x, y);
        x -= 1;
        break;
    case SCAN_CODE_D:
        if (x >= VGA_GetW() - 1)
            break;
        clearArea(x, y);
        x += 1;
        break;

    case SCAN_CODE_PAGE_UP:
        drawColor++;
        break;

    case SCAN_CODE_PAGE_DOWN:
        drawColor--;
        break;

    case SCAN_CODE_F1:
        char color = bitmap[y][x];
        if (color == -1)
            drawColor = background >> 4;
        else
            drawColor = color;
        break;

    case SCAN_CODE_TAB:
        background = drawColor << 4;
        VGA_setColor(background);
        clearAll();
        break;

    case SCAN_CODE_BACKSPACE:
        bitmap[y][x] = -1;
        break;

    case SCAN_CODE_ESCAPE:
        shouldClear = true;
        VGA_setScreenXY(0, VGA_GetH() - 5);
        VGA_setColor(GRAY << 4);
        printf("Do you wanna clear screen (y/n)?\n");
        return;

    case SCAN_CODE_F2:
        floodFill(x, y, bitmap[y][x], drawColor);
        clearAll();
        break;

    case SCAN_CODE_SPACE:
        g_shouldDraw = true;
        break;
    }
    update();
    return true;
}

void __attribute__((section(".entry"))) start(uint16_t bootDrive)
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start));
    _init(); // calling global instructers
    clearBitmap();
    HAL_Init();
    clear();
    log_info(__FILE__, "Hall initialized!");
    log_info(__FILE__, "Kernel started");
    log_info(__FILE__, "bootDriveId=%d", bootDrive);
    x = 0;
    y = 0;

    clear();

    printf("GameMachine version 1.7.1f Booted...\n\n");

    printf("Welcome to the GameMachine!\n");
    printf("Name:\n");

    const char *input = gets(SCAN_CODE_ENTER, true);
    char name[MAX_STRING_LENGTH];
    strcpy(name, input);
    putc('\n');
    printf("Welcome, %s!\n", name);

    Keyboard_Enable();

    printf("LOL, play game plz");
    clear();
    Keyboard_Subscribe(onKeyPress);
    update();
    // while (true)
    // {
    //     printf("%s_gamemachine-%d[/]\n", name, bootDrive);
    //     gets(SCAN_CODE_ENTER, true); // we already have input array ptr
    //     putc('\n');
    //     if (memcmp(input, "clear", 5) == 0)
    //         clear();
    //     else if (memcmp(input, "help", 4) == 0)
    //         printf("clear help crash_me exit\n");
    //     else if (memcmp(input, "crash_me", 8) == 0)
    //         crash_me();
    //     else if (memcmp(input, "exit", 3) == 0)
    //         goto end;
    //     else
    //     {
    //         setColor(RED);
    //         printf("Error: Invalid command: %s\n", input);
    //         setColor(WHITE);
    //     }
    // }

end:
    log_info(__FILE__, "Kernel ended\n");
    for (;;)
    {
    }
}