#include "Stopwatch.h"
Stopwatch::Stopwatch()
{
    tinit = -1e9;
}

void Stopwatch::start()
{
    tinit = GetTime();
}
double Stopwatch::ms()
{
    return (GetTime() - tinit) * 1000;
}
double Stopwatch::s()
{
    return GetTime() - tinit;
}