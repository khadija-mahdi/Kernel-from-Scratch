#include "../kernel/vga.h"

static int shift_pressed = 0;
static int ctrl_pressed = 0;
static int caps_lock = 0;

char get_ascii_char(char keycode)
{
    if (keycode == KEY_LEFT_SHIFT || keycode == KEY_RIGHT_SHIFT)
    {
        shift_pressed = !shift_pressed;
        return 0;
    }

    if (keycode == KEY_CAPS_LOCK)
    {
        caps_lock = !caps_lock;
        return 0;
    }
    if (keycode == KEY_CTRL)
    {
        ctrl_pressed = !ctrl_pressed;
        return 0;
    }

    int use_upper = (shift_pressed && !caps_lock) || (!shift_pressed && caps_lock);

    switch (keycode)
    {
    case KEY_A:
        return use_upper ? 'A' : 'a';
    case KEY_B:
        return use_upper ? 'B' : 'b';
    case KEY_C:
        return ctrl_pressed ? 0x03 : (use_upper ? 'C' : 'c');
    case KEY_D:
        return use_upper ? 'D' : 'd';
    case KEY_E:
        return use_upper ? 'E' : 'e';
    case KEY_F:
        return use_upper ? 'F' : 'f';
    case KEY_G:
        return use_upper ? 'G' : 'g';
    case KEY_H:
        return use_upper ? 'H' : 'h';
    case KEY_I:
        return use_upper ? 'I' : 'i';
    case KEY_J:
        return use_upper ? 'J' : 'j';
    case KEY_K:
        return use_upper ? 'K' : 'k';
    case KEY_L:
        return use_upper ? 'L' : 'l';
    case KEY_M:
        return use_upper ? 'M' : 'm';
    case KEY_N:
        return use_upper ? 'N' : 'n';
    case KEY_O:
        return use_upper ? 'O' : 'o';
    case KEY_P:
        return use_upper ? 'P' : 'p';
    case KEY_Q:
        return use_upper ? 'Q' : 'q';
    case KEY_R:
        return use_upper ? 'R' : 'r';
    case KEY_S:
        return use_upper ? 'S' : 's';
    case KEY_T:
        return use_upper ? 'T' : 't';
    case KEY_U:
        return use_upper ? 'U' : 'u';
    case KEY_V:
        return use_upper ? 'V' : 'v';
    case KEY_W:
        return use_upper ? 'W' : 'w';
    case KEY_X:
        return use_upper ? 'X' : 'x';
    case KEY_Y:
        return use_upper ? 'Y' : 'y';
    case KEY_Z:
        return use_upper ? 'Z' : 'z';

    // Numbers with shift symbols
    case KEY_1:
        return shift_pressed ? '!' : '1';
    case KEY_2:
        return shift_pressed ? '@' : '2';
    case KEY_3:
        return shift_pressed ? '#' : '3';
    case KEY_4:
        return shift_pressed ? '$' : '4';
    case KEY_5:
        return shift_pressed ? '%' : '5';
    case KEY_6:
        return shift_pressed ? '^' : '6';
    case KEY_7:
        return shift_pressed ? '&' : '7';
    case KEY_8:
        return shift_pressed ? '*' : '8';
    case KEY_9:
        return shift_pressed ? '(' : '9';
    case KEY_0:
        return shift_pressed ? ')' : '0';

    // Symbols with shift variants
    case KEY_MINUS:
        return shift_pressed ? '_' : '-';
    case KEY_EQUAL:
        return shift_pressed ? '+' : '=';
    case KEY_SQUARE_OPEN_BRACKET:
        return shift_pressed ? '{' : '[';
    case KEY_SQUARE_CLOSE_BRACKET:
        return shift_pressed ? '}' : ']';
    case KEY_SEMICOLON:
        return shift_pressed ? ':' : ';';
    case KEY_APOSTROPHE:
        return shift_pressed ? '"' : '\'';
    case KEY_COMMA:
        return shift_pressed ? '<' : ',';
    case KEY_DOT:
        return shift_pressed ? '>' : '.';
    case KEY_FORESLHASH:
        return shift_pressed ? '?' : '/';
    case KEY_BACKSLASH:
        return shift_pressed ? '|' : '\\';
    case KEY_GRAVE:
        return shift_pressed ? '~' : '`';

    // Special keys (no shift variants)
    case KEY_SPACE:
        return ' ';
    case KEY_ENTER:
        return '\n';
    case KEY_TAB:
        return  ctrl_pressed ? 0x10 : 0x09;
    case KEY_BACKSPACE:
        return '\b';
    case KEY_DELETE:
        return 0x7F;

    // switch btween screens
    default:
        return 0;
    }
}

void handle_key_release(char keycode)
{
    if (keycode == KEY_LEFT_SHIFT || keycode == KEY_RIGHT_SHIFT)
        shift_pressed = 0;
    else if (keycode == KEY_CTRL)
        ctrl_pressed = 0;
}
