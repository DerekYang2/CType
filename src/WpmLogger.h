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
    int total_good;
    int good;  // number of chars typed in correct words
    int error;  // number of char errors
    float error_time;  // current error is last error_time seconds
    bool current_good;  // is current word's prefix correct
    list<float> del_queue;  // delete time
    list<float> error_del_queue;  // delete time
    list<char> contribution;
    int prev_word_i;
    void reset();
    WpmLogger();
    void start();
    void push_error();  // Call for bad empty_i++
    void push_char();  // Only call for every good empty_i++
    float raw_wpm();
    float wpm();
    void update();
    void end();
    float get_elapsed();
    int current_errors();
};

extern WpmLogger wpm_logger;