#include "globals.h"
#include "StatusHandling.h"
#include "IOHandler.h"
#include "WpmLogger.h"

void update_status(char c)
{
    if (c == ' ') { update_space(); return; }
    if (words[word_i].typed.size() >= words[word_i].word.size() + 5) return;  // word too long
    char status;
    if (generated_chars[empty_i] == ' ')  // adding extra 
    {
        status = EXTRA;
        status_count.extra++;
        words[word_i].add(c);
        wpm_logger.push_error();
    } else if (c != generated_chars[empty_i]) // normal incorrect
    {
        status = INCORRECT;
        status_count.incorrect++;
        words[word_i].add(c);
        wpm_logger.push_error();
        empty_i++;
    } else  // correct
    {
        status = CORRECT;
        status_count.correct++;
        words[word_i].add(c);
        wpm_logger.push_char(true);
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
        status_count.correct++;
        io_handler.offset_x += char_dimension[' '].x;
        bool is_good = idx == words[word_i].length();  // not an extra space 
        wpm_logger.push_char(is_good), empty_i++;   // if space is in the correct position, total good char++ (affects accuracy and raw)
        words[word_i].seal_word(is_good);
        word_i++;  // go to next word
    } else if (idx != 0) // cannot press space on first index, if first index then ignore 
    {
        // JUMP TO NEXT WORD
        int space_idx = empty_i + words[word_i].length() - idx;
        for (int i = empty_i; i < space_idx; i++)  // fill missing spaces
        {
            char correct = generated_chars[i];
            char_status.push_back({ MISSING, correct });
            status_count.missing++;
            io_handler.offset_x += char_dimension[correct].x;
        }
        // space is correct 
        char_status.push_back({ CORRECT, ' ' });
        status_count.correct++;
        io_handler.offset_x += char_dimension[' '].x;
        wpm_logger.push_error();  // this space correct, but from incorrect press -> error
        // go to start next word
        empty_i = space_idx + 1;
        words[word_i].seal_word(false);
        word_i++;
    } else if (strict_space->get_selected() == "on")  // if idx == 0 but strict space is on
    {
        words[word_i].add(' ');
        char_status.push_back({ MISSING, words[word_i].word[words[word_i].idx - 1] });
        status_count.missing++;
        io_handler.offset_x += char_dimension[' '].x;
        wpm_logger.push_error();
        empty_i++;
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
        words[word_i].unseal_word();  // reset status of word
    } else
    {
        io_handler.offset_x -= char_dimension[char_status.back().second].x;
        char_status.pop_back();
        words[word_i].pop();
        if (words[word_i].idx < words[word_i].length())
            empty_i--;
    }
}
