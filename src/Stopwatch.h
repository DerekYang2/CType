#ifndef _STOPWATCH_H
#define _STOPWATCH_H
class Stopwatch
{
public:
    double tinit = -1e9;
    Stopwatch();
    void start();
    double ms();
    double s();
};
#endif