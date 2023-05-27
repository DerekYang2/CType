#include "WpmLogger.h"

#define SEC_BACK 2

WpmLogger::WpmLogger() : watch()
{
    total_wpm = 0;
    total_good = 0;
    good = 0;
}

void WpmLogger::start()
{
    total_wpm = 0;
    total_good = 0;
    good = 0;
    watch.start();
    contribution.clear();
}

void WpmLogger::end()
{
    watch.freeze();
}

void WpmLogger::push_char(bool incorrect)
{
    if (incorrect)
    {
        contribution.push_back(0);
    } else
    {
        float elapsed = watch.s();
        // Update the number of correctly typed characters
        if (starts_with(words[word_i].word, words[word_i].typed))
        {
            good++;
            del_queue.push_back(elapsed + SEC_BACK);  // when to delete this char's contribution
        }
        total_good++;
        contribution.push_back(1);
    }
}

void WpmLogger::pop_char()
{
    total_good -= contribution.back();
    contribution.pop_back();
}

void WpmLogger::update()
{
    while (!del_queue.empty() && del_queue.front() <= watch.s())  // time reached
    {
        del_queue.pop_front();
        good--;
    }
}

float WpmLogger::current_wpm()
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
    return 60 * (float)total_good * 0.2f / elapsed; 
}

