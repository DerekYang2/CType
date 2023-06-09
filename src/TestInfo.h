#pragma once
#include "WpmLogger.h"
#include "Graph.h"
struct TestInfo {
    Stopwatch watch;
    int time;
    // wpm graph info
    vector<float> wpm_record, raw_wpm_record;
    float sec_per_check;  // seconds per check
    TestInfo();
    void init(int test_time);
    void update();
};
extern TestInfo test_info;