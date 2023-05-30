#include "WpmLogger.h"

#define SEC_BACK 4

WpmLogger::WpmLogger() : watch()
{
    reset();
}

void WpmLogger::reset()
{
    total_good = 0;
    good = 0;
    prev_word_i = 0;
    contribution.clear();
    contribution.push_back(0); // start with 0 on first word
}

void WpmLogger::start()
{
    watch.start();
}

void WpmLogger::end()
{
    watch.freeze();
}

void WpmLogger::push_char()
{
    float elapsed = watch.s();
    // Update the number of correctly typed characters
    if (words[word_i].current_correct())
    {
        good++;
        del_queue.push_back(elapsed + SEC_BACK);  // when to delete this char's contribution
    }
}

void WpmLogger::update()
{
    while (!del_queue.empty() && del_queue.front() <= watch.s())  // time reached
    {
        del_queue.pop_front();
        good--;
    }
    
    // update total_good 
    if (word_i < prev_word_i)  // deleted
    {
        int del_amt = prev_word_i - word_i + 1;  // +1 to delete word_i too
        while (del_amt--)
        {
            total_good -= contribution.back();
            contribution.pop_back();
        }
        // reload word_i
        contribution.push_back(words[word_i].contribution());
        total_good += contribution.back();
    } else
    {
        // clear prev_word_i 
        total_good -= contribution.back();
        contribution.pop_back();
        // reload prev_word_i to word_i
        for (int i = prev_word_i; i <= word_i; i++)
        {
            contribution.push_back(words[i].contribution());
            total_good += contribution.back();
        }
    }
    prev_word_i = word_i;
}

float WpmLogger::raw_wpm()
{
    return 60 * (float)good * 0.2f / (float)SEC_BACK;
}

/**
 * FORMULA:
 * 60 * (total_good / 5) / seconds passed
 * 60 * (number of chars in correct words/5) / seconds passed -> assuming average 5 chars per word
*/
float WpmLogger::wpm()
{
    float elapsed = watch.s();
    if (elapsed <= 1) return 0;  // too early
    return 12 * (float)total_good / elapsed;  // 60 / 5 -> 12
}

float WpmLogger::elapsed()
{
    return watch.s();
}