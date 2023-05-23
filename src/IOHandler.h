#pragma once
#include "TextDrawer.h"
#define inactive_time 2
struct IOHandler {
    int inactive_frames = 0;
    int back_frames = 0;
    float offset_x = 0;
    float offset_vel = 0, vel_target = 0;
    bool handled_press[CHAR_MAX + 1];
    IOHandler();
    void update();
};

extern IOHandler io_handler;