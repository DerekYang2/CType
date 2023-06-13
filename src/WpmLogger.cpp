#include "WpmLogger.h"

#define SEC_BACK 5

WpmLogger::WpmLogger() : watch()
{
    reset();
}

void WpmLogger::reset()
{
    total_good = 0;
    total_typed = 0;
    elapsed = 0;
    good = 0;
    raw_good = 0;
    prev_word_i = 0;
    contribution.clear();
    contribution.push_back(0); // start with 0 on first word
    del_queue.clear();
    error_log.clear();
}

void WpmLogger::start()
{
    watch.start();
}

void WpmLogger::end()
{
    watch.freeze();
    elapsed = watch.s();
}

/**
 * good_char is true if the current character pressed is correct
 * possible for good_char = true but raw_good doesn't change because the full word may not be correct
*/
void WpmLogger::push_char(bool good_char)
{
    total_typed++;
    if (good_char)  // if at least current char is correct
    {
        total_good++;
    } else  // if current char is incorrect
    {
        error_log.push_back(elapsed);
    }
    
    if (words[word_i].current_correct())  // no previous typos in prefix of current word
    {
        raw_good++;
        del_queue.push_back(elapsed + SEC_BACK);  // when to delete this char's contribution
    } 
}

void WpmLogger::push_error()
{
    total_typed++;
    error_log.push_back(elapsed);
}

void WpmLogger::update()
{
    elapsed = watch.s();  // update elapsed
    
    while (!del_queue.empty() && del_queue.front() <= elapsed)  // time reached
    {
        del_queue.pop_front();
        raw_good--;
    }

    // update total_good 
    if (word_i < prev_word_i)  // deleted
    {
        int del_amt = prev_word_i - word_i + 1;  // +1 to delete word_i too
        while (del_amt--)
        {
            good -= contribution.back();
            contribution.pop_back();
        }
        // reload word_i
        contribution.push_back(words[word_i].contribution());
        good += contribution.back();
    } else
    {
        // clear prev_word_i 
        good -= contribution.back();
        contribution.pop_back();
        // reload prev_word_i to word_i
        for (int i = prev_word_i; i <= word_i; i++)
        {
            contribution.push_back(words[i].contribution());
            good += contribution.back();
        }
    }
    prev_word_i = word_i;
}

float WpmLogger::instant_wpm()
{
    if (elapsed <= 1) return 0;
    return 12 * (float)raw_good / min(elapsed, (float)SEC_BACK);
}

float WpmLogger::raw_wpm()
{
    if (elapsed <= 1) return 0;
    return 12 * (float)total_typed / elapsed;
}

/**
 * FORMULA:
 * 60 * (good / 5) / seconds passed
 * 60 * (number of chars in correct words/5) / seconds passed -> assuming average 5 chars per word
*/
float WpmLogger::wpm()
{
    if (elapsed <= 1) return 0;  // too early
    return 12 * (float)good / elapsed;  // 60 / 5 -> 12
}

float WpmLogger::accuracy()
{
    return (float)total_good / total_typed;
}

float WpmLogger::get_elapsed()
{
    return elapsed;
}