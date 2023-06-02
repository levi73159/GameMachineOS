#include "keyboard.h"

bool isShiftKeyPressed()
{
    // Read the keyboard status register to check the state of the shift modifier
    uint8_t status = i686_inb(KEYBOARD_STATUS_PORT);

    // Check the status byte for the state of the left and right shift keys
    bool leftShiftPressed = (status & KEYBOARD_STATUS_LEFT_SHIFT) != 0;
    bool rightShiftPressed = (status & KEYBOARD_STATUS_RIGHT_SHIFT) != 0;

    // Return true if either the left or right shift key is pressed
    return leftShiftPressed || rightShiftPressed;
}

char convertScanCodeToChar(uint8_t scanCode, bool shiftPressed)
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