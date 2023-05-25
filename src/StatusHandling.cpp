#include "globals.h"
#include "StatusHandling.h"
#include "IOHandler.h"

void update_status(char c)
{
    if (c == ' ') { update_space(); return; }
    
    char status;
    if (generated_chars[empty_i] == ' ')  // adding extra 
    {
        status = EXTRA;
        words[word_i].add(c);
    } else if (c != generated_chars[empty_i]) // normal incorrect
    {
        status = INCORRECT;
        words[word_i].add(c);
        empty_i++;
    } else  // correct
    {
        status = CORRECT;
        words[word_i].add(c);
        empty_i++;
    }
    char_status.push_back({ status, c });
    io_handler.offset_x += char_dimension[c].x;
}

void update_space()
{
    int idx = words[word_i].idx;
    if (idx >= words[word_i].length())  // on the space, or extra char then space
    {
        char_status.push_back({ CORRECT, ' ' });
        io_handler.offset_x += char_dimension[' '].x;
        empty_i++;
        word_i++;  // go to next word
    } else if (idx != 0) // cannot be first index, if first index then ignore 
    {
        // JUMP TO NEXT WORD
        int space_idx = empty_i + words[word_i].length() - idx;
        for (int i = empty_i; i < space_idx; i++)  // fill missing 
        {
            char correct = generated_chars[i];
            char_status.push_back({ MISSING, correct });
            io_handler.offset_x += char_dimension[correct].x;
        }
        // space is correct 
        char_status.push_back({ CORRECT, ' ' });
        io_handler.offset_x += char_dimension[' '].x;
        // go to start next word
        empty_i = space_idx + 1;
        word_i++;
    }
}

void update_backspace()
{
    if (empty_i == 0) return;
    if (words[word_i].idx == 0)
    {
        if (words[word_i - 1].correct()) return; // cannot backspace new word if previous is correct
        int del_amt = max(0, words[word_i - 1].length() - words[word_i-1].idx) + 1;
        for (int i = 0; i < del_amt; i++)  // del amt times
        {
            io_handler.offset_x -= char_dimension[char_status.back().second].x;
            char_status.pop_back();
        }
        empty_i -= del_amt;
        word_i--;
    } else
    {
        io_handler.offset_x -= char_dimension[char_status.back().second].x;
        char_status.pop_back();
        words[word_i].pop();
        if (words[word_i].idx < words[word_i].length())
            empty_i--;
    }
}
