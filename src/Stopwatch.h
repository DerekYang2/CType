#ifndef _STOPWATCH_H
#define _STOPWATCH_H
#include "raylib.h"
class Stopwatch
{
public:
    double t_init;
    double t_end;
    Stopwatch();
    void start();
    void freeze();
    double ms();
    double s();
};
#endif