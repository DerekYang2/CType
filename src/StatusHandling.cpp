#include "globals.h"
#include "StatusHandling.h"
#include "IOHandler.h"

void update_status(char c)
{
    char status = CORRECT;
    char_status.push_back({ status, c });
    io_handler.offset_x += char_dimension[c].x;
    empty_i++;
}

void update_space()
{
    char status = CORRECT;
    char_status.push_back({ status, ' ' });
    io_handler.offset_x += char_dimension[' '].x;
    empty_i++;
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
