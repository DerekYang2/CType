#include "TestInfo.h"
#define sample_hz 2
TestInfo::TestInfo() {}

#define n_func(t) ((t) >= 120 ? 50 : (-0.369396 * (t) + 9.26095 * sqrt(t) + -7.2506))

void TestInfo::init(int test_time)
{
    time = test_time;
    wpm_record.clear();
    raw_wpm_record.clear();
    error_record.clear();
    watch.start();
    float n = n_func(time);
    sec_per_check = time / (n - 1);
}

void TestInfo::update()
{
    // get wpm sample
    if (watch.s() >= sec_per_check)
    {
        watch.start();  // reset
        if (wpm_logger.get_elapsed() >= 1)  // cannot be too early
        {
            wpm_record.push_back(wpm_logger.wpm());
            raw_wpm_record.push_back(wpm_logger.instant_wpm());
        }
    }
}

void TestInfo::update_graph(Graph* graph)
{
    graph->reset();
    graph->set_time(time);
    graph->config_max(wpm_record);
    graph->config_max(raw_wpm_record, RAW);
    graph->set_plot(wpm_record);
    graph->set_plot(raw_wpm_record, RAW);
    graph->set_error(wpm_logger.error_log);
}
