#include "TestInfo.h"

#define sample_hz 2

TestInfo::TestInfo() {}

void TestInfo::init(int test_time)
{
    time = test_time;
    wpm_record.clear();
    raw_wpm_record.clear();
    watch.start();
}

void TestInfo::update()
{
    // get wpm sample
    if (watch.s() * sample_hz >= 1)
    {
        watch.start();  // reset
        if (wpm_logger.elapsed() >= 1)  // cannot be too early    
            wpm_record.push_back(wpm_logger.wpm());
        raw_wpm_record.push_back(wpm_logger.raw_wpm());
    }
}