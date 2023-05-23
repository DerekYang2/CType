#include "globals.h"
#include "StatusHandling.h"
#include "IOHandler.h"

void update_status(char c)
{
    if (c == ' ') { update_space(); return; }
    
    char status = CORRECT;
    if (c != generated_chars[empty_i])
    {
        status = INCORRECT;
    }
    char_status.push_back({ status, c });
    io_handler.offset_x += char_dimension[c].x;
    empty_i++;
}

void update_space()
{
    if (' ' != generated_chars[empty_i])  // wrong space
    {
        char correct = generated_chars[empty_i];
        char_status.push_back({ MISSING, correct });
        io_handler.offset_x += char_dimension[correct].x;
        empty_i++;
    } else
    {
        
        char_status.push_back({ CORRECT, ' ' });
        io_handler.offset_x += char_dimension[' '].x;
        empty_i++;
    }
}

void update_backspace()
{
    if (empty_i > 0)
    {
        io_handler.offset_x -= char_dimension[char_status.back().second].x;
        char_status.pop_back();
        empty_i--;
    }
}
