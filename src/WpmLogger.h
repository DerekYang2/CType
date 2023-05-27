#pragma once
#include "globals.h"
#include "Stopwatch.h"
/**
 * WPM: ((number of chars typed in correct words)/5) / sec_back
 * words / sec_back * 60 = wpm
*/

struct WpmLogger {
    Stopwatch watch;
    int total_wpm, total_good;
    int good;  // number of chars typed in correct words
    bool current_good;  // is current word's prefix correct
    std::deque<float> del_queue;  // {word index, time to delete}
    vector<bool> contribution;  // contribution of character, should match generated_chars and empty_i
    WpmLogger();
    void start();
    void push_char(bool correct);  // Only call for every empty_i++
    void pop_char(); // Only call for every empty_i--
    float current_wpm();
    float wpm();
    void update();
    void end();
};

extern WpmLogger wpm_logger;