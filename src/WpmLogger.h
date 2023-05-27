#pragma once
#include "globals.h"
#include "Stopwatch.h"
/**
 * WPM: ((number of chars typed in correct words)/5) / sec_back
 * words / sec_back * 60 = wpm
*/

struct WpmLogger {
    Stopwatch watch;
    int total_good;
    int good;  // number of chars typed in correct words
    bool current_good;  // is current word's prefix correct
    list<float> del_queue;  // {word index, time to delete}
    list<char> contribution;
    int prev_word_i;
    WpmLogger();
    void start();
    void push_char();  // Only call for every good empty_i++
    float current_wpm();
    float wpm();
    void update();
    void end();
};

extern WpmLogger wpm_logger;