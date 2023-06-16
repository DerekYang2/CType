#pragma once
#include "WpmLogger.h"
#include "Graph.h"
struct TestInfo {
    Stopwatch watch;
    int time;
    // wpm graph info
    vector<float> wpm_record, raw_wpm_record;
    vector<int> error_record;
    float sec_per_check;  // seconds per check
    TestInfo();
    void init(int test_time);
    void update();
    float variation();
    float get_cov(vector<float>& sample);  // coefficient of variation
    void update_graph(Graph* graph);
};
extern TestInfo test_info;
