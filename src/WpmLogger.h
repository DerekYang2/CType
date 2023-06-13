#pragma once
#include "globals.h"
#include "Stopwatch.h"
/**
 * WPM: ((number of chars typed in correct words)/5) / sec_back
 * words / sec_back * 60 = wpm
*/

struct WpmLogger {
    Stopwatch watch;
    float elapsed;
    int total_good;  // number of correct chars typed in any situation
    int total_typed;
    int good; // number of chars typed in correct words (normal)
    int raw_good;  // number of chars typed in correct words (instant, last 5 sec)
    bool current_good;  // is current word's prefix correct
    list<float> del_queue;  // delete time
    vector<float> error_log;
    list<char> contribution;
    int prev_word_i;
    void reset();
    WpmLogger();
    void start();
    void push_error();  // Call for bad empty_i++
    void push_char(bool good_char);  // Only call for every good empty_i++
    float instant_wpm();  // instantaneous wpm (last 5 sec)
    float wpm(); // normal wpm
    float raw_wpm();  // do not count errors
    float accuracy();
    void update();
    void end();
    float get_elapsed();
};

extern WpmLogger wpm_logger;