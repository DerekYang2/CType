#ifndef _STOPWATCH_H
#define _STOPWATCH_H
#include "raylib.h"
class Stopwatch
{
public:
    double tinit;
    Stopwatch();
    void start();
    double ms();
    double s();
};
#endif