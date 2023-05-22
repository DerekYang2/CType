#include "Stopwatch.h"
#include "raylib.h"
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