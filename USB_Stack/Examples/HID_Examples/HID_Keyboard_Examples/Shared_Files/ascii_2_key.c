/**
 * @file ascii_2_key.c
 * @brief Convert ASCII to HID Key values.
 * @author John Izzard
 * @date 05/06/2020
 * 
 * USB uC - HID Examples.
 * Copyright (C) 2017-2020  John Izzard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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