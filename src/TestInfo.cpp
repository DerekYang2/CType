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

// coefficient of variation mapped between 0 and 1
float TestInfo::variation()
{
    /**
     * technically cov(raw_wpm)/cov(worst case sample)
     * worst case sample of half 0, half max wpm gives cov of 1, so return cov(raw_wpm)/1 
     */
    return get_cov(raw_wpm_record);
}

float TestInfo::get_cov(vector<float>& sample)
{
    float mean = 0;
    for (auto& x : sample)
        mean += x;
    mean /= sample.size();
    
    float stdev = 0;
    for (auto& x : sample)
        stdev += (x - mean) * (x - mean);
    stdev /= sample.size();
    stdev = sqrt(stdev);

    float coef_var = stdev / mean;
    return coef_var;
}

void TestInfo::update_graph(Graph* graph)
{
    graph->reset();
    graph->set_time(time);
    graph->config_max(wpm_record);
    graph->config_max(raw_wpm_record, RAW);
    graph->set_plot(wpm_record, NORMAL);
    graph->set_plot(raw_wpm_record, RAW);
    graph->set_error(wpm_logger.error_log);
}
