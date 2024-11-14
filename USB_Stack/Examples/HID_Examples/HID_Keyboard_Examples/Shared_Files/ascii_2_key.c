/**
 * @file ascii_2_key.c
 * @brief Convert ASCII to HID Key values.
 * @author John Izzard
 * @date 2024-11-14
 * 
 * USB uC - HID Examples.
 * Copyright (C) 2017-2024 John Izzard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
#include <stdint.h>
#include <stdbool.h>
#include "ascii_2_key.h"

#define IS_CHARACTER      (val > 31 && val < 127)
#define IS_CAPITAL_LETTER (val > 64 && val < 91)
#define IS_LOWER_LETTER   (val > 96 && val < 123)
#define IS_NUMBER         (val > 47 && val < 58)

#define IS_SYMBOL_AREA_1  (val < 48)
#define IS_SYMBOL_AREA_2  (val < 65)
#define IS_SYMBOL_AREA_3  (val < 97)
#define IS_SYMBOL_AREA_4  (val < 127)

Key_Result_t g_key_result;

const uint8_t symbol_lookup_1[16] =
{
    KEY_SPACE,      // ' '
    KEY_1,          // '!'
    KEY_APOSTROPHE, // '\"'
    KEY_3,          // '#'
    KEY_4,          // '$'
    KEY_5,          // '%'
    KEY_7,          // '&'
    KEY_APOSTROPHE, // '\''
    KEY_9,          // '('
    KEY_0,          // ')'
    KEY_8,          // '*'
    KEY_EQUAL,      // '+'
    KEY_COMMA,      // ','
    KEY_MINUS,      // '-'
    KEY_DOT,        // '.'
    KEY_SLASH       // '/'
};

const uint8_t symbol_lookup_2[7] =
{
    KEY_SEMICOLON,  // ':'
    KEY_SEMICOLON,  // ';'
    KEY_COMMA,      // '<'
    KEY_EQUAL,      // '='
    KEY_DOT,        // '>'
    KEY_SLASH,      // '?'
    KEY_2           // '@'
};

const uint8_t symbol_lookup_3[6] =
{
    KEY_LEFTBRACE,  // '['
    KEY_BACKSLASH,  // '\'
    KEY_RIGHTBRACE, // ']'
    KEY_6,          // '^'
    KEY_MINUS,      // '_'
    KEY_GRAVE       // '`'
};

const uint8_t symbol_lookup_4[4] =
{
    KEY_LEFTBRACE,  // '{'
    KEY_BACKSLASH,  // '|'
    KEY_RIGHTBRACE, // '{'
    KEY_GRAVE       // '~'
};

void ascii_2_key(uint8_t val)
{
    g_key_result.Modifier = 0;
    g_key_result.KeyCode  = KEY_NULL;
    
    if(IS_CHARACTER)
    {
        if(IS_LOWER_LETTER)
        {
            g_key_result.KeyCode = val - ('a' - KEY_A); // 'a' - KEY_A = 97 - 4 = 93.
        }
        else if(IS_CAPITAL_LETTER)
        {
            g_key_result.Modifier = MOD_KEY_LEFTSHIFT;
            g_key_result.KeyCode = val - ('A' - KEY_A);  // 'A' - KEY_A = 65 - 4 = 61.
        }
        else if(IS_NUMBER)
        {
            if(val == '0') g_key_result.KeyCode = KEY_0;
            else g_key_result.KeyCode = val - ('1' - KEY_1);  // '1' - KEY_1 = 49 - 30 = 19.
        }
        else
        {
            if(IS_SYMBOL_AREA_1)
            {
                if((val != ' ') && 
                   (val != '\'') && 
                   (val != ',') &&
                   (val != '-') &&
                   (val != '.') &&
                   (val != '/')) g_key_result.Modifier = MOD_KEY_LEFTSHIFT;
                g_key_result.KeyCode = symbol_lookup_1[val - ' '];
            }
            else if(IS_SYMBOL_AREA_2)
            {
                if((val != ';')&&(val != '=')) g_key_result.Modifier = MOD_KEY_LEFTSHIFT;
                g_key_result.KeyCode = symbol_lookup_2[val - ':'];
            }
            else if(IS_SYMBOL_AREA_3)
            {
                if((val == '^')||(val == '_')) g_key_result.Modifier = MOD_KEY_LEFTSHIFT;
                g_key_result.KeyCode = symbol_lookup_3[val - '['];
            }
            else if(IS_SYMBOL_AREA_4)
            {
                g_key_result.Modifier = MOD_KEY_LEFTSHIFT;
                g_key_result.KeyCode = symbol_lookup_4[val - '{'];
            }
        }
    }
    else
    {
        switch(val)
        {
            case ELC: // End of Line Character set in header file.
                g_key_result.KeyCode = KEY_ENTER;
                break;
            case '\t':
                g_key_result.KeyCode = KEY_TAB;
                break;
            case '\b':
                g_key_result.KeyCode = KEY_BACKSPACE;
                break;
        }
    }
}