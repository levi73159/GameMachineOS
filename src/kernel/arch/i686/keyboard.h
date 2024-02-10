#pragma once
#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "isr.h"

#define SCAN_CODE_ESCAPE 0x01
#define SCAN_CODE_1 0x02
#define SCAN_CODE_2 0x03
#define SCAN_CODE_3 0x04
#define SCAN_CODE_4 0x05
#define SCAN_CODE_5 0x06
#define SCAN_CODE_6 0x07
#define SCAN_CODE_7 0x08
#define SCAN_CODE_8 0x09
#define SCAN_CODE_9 0x0A
#define SCAN_CODE_0 0x0B
#define SCAN_CODE_MINUS 0x0C
#define SCAN_CODE_EQUAL 0x0D
#define SCAN_CODE_BACKSPACE 0x0E
#define SCAN_CODE_TAB 0x0F
#define SCAN_CODE_Q 0x10
#define SCAN_CODE_W 0x11
#define SCAN_CODE_E 0x12
#define SCAN_CODE_R 0x13
#define SCAN_CODE_T 0x14
#define SCAN_CODE_Y 0x15
#define SCAN_CODE_U 0x16
#define SCAN_CODE_I 0x17
#define SCAN_CODE_O 0x18
#define SCAN_CODE_P 0x19
#define SCAN_CODE_LEFT_BRACKET 0x1A
#define SCAN_CODE_RIGHT_BRACKET 0x1B
#define SCAN_CODE_ENTER 0x1C
#define SCAN_CODE_LEFT_CONTROL 0x1D
#define SCAN_CODE_A 0x1E
#define SCAN_CODE_S 0x1F
#define SCAN_CODE_D 0x20
#define SCAN_CODE_F 0x21
#define SCAN_CODE_G 0x22
#define SCAN_CODE_H 0x23
#define SCAN_CODE_J 0x24
#define SCAN_CODE_K 0x25
#define SCAN_CODE_L 0x26
#define SCAN_CODE_SEMICOLON 0x27
#define SCAN_CODE_SINGLE_QUOTE 0x28
#define SCAN_CODE_BACK_TICK 0x29
#define SCAN_CODE_LEFT_SHIFT 0x2A
#define SCAN_CODE_RELEASED_LEFT_SHIFT 0xAA
#define SCAN_CODE_BACKSLASH 0x2B
#define SCAN_CODE_Z 0x2C
#define SCAN_CODE_X 0x2D
#define SCAN_CODE_C 0x2E
#define SCAN_CODE_V 0x2F
#define SCAN_CODE_B 0x30
#define SCAN_CODE_N 0x31
#define SCAN_CODE_M 0x32
#define SCAN_CODE_COMMA 0x33
#define SCAN_CODE_PERIOD 0x34
#define SCAN_CODE_SLASH 0x35
#define SCAN_CODE_RIGHT_SHIFT 0x36
#define SCAN_CODE_RELEASED_RIGHT_SHIFT 0xb6
#define SCAN_CODE_KEYPAD_ASTERISK 0x37
#define SCAN_CODE_LEFT_ALT 0x38
#define SCAN_CODE_SPACE 0x39
#define SCAN_CODE_CAPS_LOCK 0x3A
#define SCAN_CODE_F1 0x3B
#define SCAN_CODE_F2 0x3C
#define SCAN_CODE_F3 0x3D
#define SCAN_CODE_F4 0x3E
#define SCAN_CODE_F5 0x3F
#define SCAN_CODE_F6 0x40
#define SCAN_CODE_F7 0x41
#define SCAN_CODE_F8 0x42
#define SCAN_CODE_F9 0x43
#define SCAN_CODE_F10 0x44
#define SCAN_CODE_NUM_LOCK 0x45
#define SCAN_CODE_SCROLL_LOCK 0x46
#define SCAN_CODE_KEYPAD_7 0x47
#define SCAN_CODE_KEYPAD_8 0x48
#define SCAN_CODE_KEYPAD_9 0x49
#define SCAN_CODE_KEYPAD_MINUS 0x4A
#define SCAN_CODE_KEYPAD_4 0x4B
#define SCAN_CODE_KEYPAD_5 0x4C
#define SCAN_CODE_KEYPAD_6 0x4D
#define SCAN_CODE_KEYPAD_PLUS 0x4E
#define SCAN_CODE_KEYPAD_1 0x4F
#define SCAN_CODE_KEYPAD_2 0x50
#define SCAN_CODE_KEYPAD_3 0x51
#define SCAN_CODE_KEYPAD_0 0x52
#define SCAN_CODE_KEYPAD_PERIOD 0x53
#define SCAN_CODE_F11 0x57
#define SCAN_CODE_F12 0x58
#define SCAN_CODE_PAGE_UP 0x49
#define SCAN_CODE_PAGE_DOWN 0x51

// Keyboard data register port
#define KEYBOARD_DATA_PORT 0x60
// Keyboard status register port
#define KEYBOARD_STATUS_PORT 0x64

// Keyboard status bits
#define KEYBOARD_STATUS_LEFT_SHIFT (1 << 0)
#define KEYBOARD_STATUS_RIGHT_SHIFT (1 << 1)

typedef struct
{
    uint8_t keyCode;
    char keyChar;
    bool release;
} Keyboard_Key;

typedef bool (*keyPress)(uint8_t, Registers *);

char Keyboard_ScanCodeToChar(uint8_t scanCode, bool shiftPressed);
bool Keyboard_ShiftKeyPressed();
uint8_t Keyboard_GetKeyCode(bool new, bool remove);

void Keyboard_ClearBuffer();
void i686_Keyboard_Initialize();
void Keyboard_Enable();
void Keyboard_Disable();
bool Keyboard_Subscribe(keyPress func);
bool Keyboard_Extract(uint8_t *key);