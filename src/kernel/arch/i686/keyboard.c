#include "keyboard.h"
#include <stdio.h>
#include <memory.h>
#include "pic.h"
#include "i8259.h"
#include "isr.h"
#include <util/arrays.h>
#include <debug.h>

#define BUFFER_LENGTH 1024
#define SUBSCRIBE_LENGTH 5
uint8_t g_KeyCodeBuffer[BUFFER_LENGTH];
PICDriver *g_Driver = NULL;
keyPress g_KeyPresses[SUBSCRIBE_LENGTH];

bool Keyboard_Subscribe(keyPress func)
{
    for (uint16_t i = 0; i < SUBSCRIBE_LENGTH; i++)
    {
        if (g_KeyPresses[i] != NULL)
            continue;

        g_KeyPresses[i] = func;
        return true;
    }
    log_warn("KEYBOARD", "No slots to subscribe the function to!");
    return false;
}

void shiftLeft(uint8_t *array, unsigned int length)
{
    for (unsigned int i = 0; i < length - 1; i++)
    {
        array[i] = array[i + 1];
    }
}

void keyboardHandler(Registers *regs)
{
    // Read the keyboard input data from the appropriate I/O port(s)
    uint8_t data = i686_inb(KEYBOARD_DATA_PORT);
    // Check the most significant bit of the data byte
    // If it is set (1), it indicates a key release event, so we ignore it
    bool keyCodeUpdated = false;

    for (uint16_t i = 0; i < SUBSCRIBE_LENGTH; i++)
    {
        if (g_KeyPresses[i] == NULL)
            break;

        // if false it will append the key to the list, if true it wont add the key, saying that it was handle
        if (g_KeyPresses[i](data, regs))
            return;
    }

    int indexToFree;
    if (getFree(g_KeyCodeBuffer, BUFFER_LENGTH, &indexToFree))
    {
        g_KeyCodeBuffer[indexToFree] = data;
        keyCodeUpdated = true;
    }

    if (keyCodeUpdated)
        return;
    shiftLeft(g_KeyCodeBuffer, BUFFER_LENGTH);
    g_KeyCodeBuffer[BUFFER_LENGTH - 1] = data;
    keyCodeUpdated = true;
}

bool Keyboard_Extract(uint8_t* key)
{
    if (*key & 0x80)
    {
        *key &= 0x7F; // Extract lower 7 bits
        // Now, keyCode contains the value of the key without the high bit (key released)
        // You can use keyCode as needed
        return true;
    }
    return false;
}

void i686_Keyboard_Initialize()
{
    i686_IRQ_RegisterHandler(1, keyboardHandler);
    g_Driver = i8259_GetDriver();
}

bool Keyboard_ShiftKeyPressed()
{
    // Read the keyboard status register to check the state of the shift modifier
    uint8_t status = i686_inb(KEYBOARD_STATUS_PORT);

    // Check the status byte for the state of the left and right shift keys
    bool leftShiftPressed = (status & KEYBOARD_STATUS_LEFT_SHIFT) != 0;
    bool rightShiftPressed = (status & KEYBOARD_STATUS_RIGHT_SHIFT) != 0;

    // Return true if either the left or right shift key is pressed
    return leftShiftPressed || rightShiftPressed;
}

char Keyboard_ScanCodeToChar(uint8_t scanCode, bool shiftPressed)
{
    bool capitalize = shiftPressed;

    switch (scanCode)
    {
    case SCAN_CODE_ESCAPE:
        return '\033';
    case SCAN_CODE_1:
        return capitalize ? '!' : '1';
    case SCAN_CODE_2:
        return capitalize ? '@' : '2';
    case SCAN_CODE_3:
        return capitalize ? '#' : '3';
    case SCAN_CODE_4:
        return capitalize ? '$' : '4';
    case SCAN_CODE_5:
        return capitalize ? '%' : '5';
    case SCAN_CODE_6:
        return capitalize ? '^' : '6';
    case SCAN_CODE_7:
        return capitalize ? '&' : '7';
    case SCAN_CODE_8:
        return capitalize ? '*' : '8';
    case SCAN_CODE_9:
        return capitalize ? '(' : '9';
    case SCAN_CODE_0:
        return capitalize ? ')' : '0';
    case SCAN_CODE_MINUS:
        return capitalize ? '_' : '-';
    case SCAN_CODE_EQUAL:
        return capitalize ? '+' : '=';
    case SCAN_CODE_BACKSPACE:
        return '\b';
    case SCAN_CODE_TAB:
        return '\t';
    case SCAN_CODE_Q:
        return capitalize ? 'Q' : 'q';
    case SCAN_CODE_W:
        return capitalize ? 'W' : 'w';
    case SCAN_CODE_E:
        return capitalize ? 'E' : 'e';
    case SCAN_CODE_R:
        return capitalize ? 'R' : 'r';
    case SCAN_CODE_T:
        return capitalize ? 'T' : 't';
    case SCAN_CODE_Y:
        return capitalize ? 'Y' : 'y';
    case SCAN_CODE_U:
        return capitalize ? 'U' : 'u';
    case SCAN_CODE_I:
        return capitalize ? 'I' : 'i';
    case SCAN_CODE_O:
        return capitalize ? 'O' : 'o';
    case SCAN_CODE_P:
        return capitalize ? 'P' : 'p';
    case SCAN_CODE_LEFT_BRACKET:
        return capitalize ? '{' : '[';
    case SCAN_CODE_RIGHT_BRACKET:
        return capitalize ? '}' : ']';
    case SCAN_CODE_ENTER:
        return '\n';
    case SCAN_CODE_A:
        return capitalize ? 'A' : 'a';
    case SCAN_CODE_S:
        return capitalize ? 'S' : 's';
    case SCAN_CODE_D:
        return capitalize ? 'D' : 'd';
    case SCAN_CODE_F:
        return capitalize ? 'F' : 'f';
    case SCAN_CODE_G:
        return capitalize ? 'G' : 'g';
    case SCAN_CODE_H:
        return capitalize ? 'H' : 'h';
    case SCAN_CODE_J:
        return capitalize ? 'J' : 'j';
    case SCAN_CODE_K:
        return capitalize ? 'K' : 'k';
    case SCAN_CODE_L:
        return capitalize ? 'L' : 'l';
    case SCAN_CODE_SEMICOLON:
        return capitalize ? ':' : ';';
    case SCAN_CODE_SINGLE_QUOTE:
        return capitalize ? '"' : '\'';
    case SCAN_CODE_BACK_TICK:
        return capitalize ? '~' : '`';
    case SCAN_CODE_BACKSLASH:
        return capitalize ? '|' : '\\';
    case SCAN_CODE_Z:
        return capitalize ? 'Z' : 'z';
    case SCAN_CODE_X:
        return capitalize ? 'X' : 'x';
    case SCAN_CODE_C:
        return capitalize ? 'C' : 'c';
    case SCAN_CODE_V:
        return capitalize ? 'V' : 'v';
    case SCAN_CODE_B:
        return capitalize ? 'B' : 'b';
    case SCAN_CODE_N:
        return capitalize ? 'N' : 'n';
    case SCAN_CODE_M:
        return capitalize ? 'M' : 'm';
    case SCAN_CODE_COMMA:
        return capitalize ? '<' : ',';
    case SCAN_CODE_PERIOD:
        return capitalize ? '>' : '.';
    case SCAN_CODE_SLASH:
        return capitalize ? '?' : '/';
    case SCAN_CODE_SPACE:
        return ' ';
    default:
        break;
    }

    return '\0'; // Return null character for unknown scan codes
}

uint8_t Keyboard_GetKeyCode(bool new, bool remove)
{
    uint8_t code = 0;
    if (new)
    {
        uint32_t index;
        bool isLast = !getFree(g_KeyCodeBuffer, BUFFER_LENGTH, &index);
        if (isLast)
            index = BUFFER_LENGTH - 1;
        else
            index--;
        code = g_KeyCodeBuffer[index];
        if (remove && code != 0)
            g_KeyCodeBuffer[index] = 0;
    }
    else
    {
        code = g_KeyCodeBuffer[0];
        if (remove && code != 0)
            shiftLeft(g_KeyCodeBuffer, BUFFER_LENGTH);
    }
    return code;
}

void Keyboard_ClearBuffer()
{
    *g_KeyCodeBuffer = memset(g_KeyCodeBuffer, 0, BUFFER_LENGTH);
}

void Keyboard_Enable()
{
    g_Driver->Unmask(1);
}

void Keyboard_Disable()
{
    g_Driver->Mask(1);
}