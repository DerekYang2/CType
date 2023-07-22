#pragma once
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