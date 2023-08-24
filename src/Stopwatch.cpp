#include "raylib.h"
#include "Stopwatch.h"

Stopwatch::Stopwatch()
{
    t_init = -1e9;
    t_end = -1;
}

void Stopwatch::start()
{
    t_init = GetTime();
    t_end = -1;
}

void Stopwatch::freeze()
{
    t_end = GetTime();
}

double Stopwatch::ms()
{
    return ((t_end == -1 ? GetTime() : t_end) - t_init) * 1000;
}
double Stopwatch::s()
{
     return ((t_end == -1 ? GetTime() : t_end) - t_init);
}