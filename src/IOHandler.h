#pragma once
#include "TextDrawer.h"
#include "StatusHandling.h"
#include "WpmLogger.h"
#define inactive_time 1
struct IOHandler {
    int inactive_frames = 0;
    int back_frames = 0;
    float offset_x = 0;
    float offset_vel = 0, vel_target = 0;
    bool handled_press[CHAR_MAX + 1];
    function<void(char)> add_function;
    function<void()> back_function;
    IOHandler();
    IOHandler(function<void(char)> add_function, function<void()> back_function);
    void update();
};

extern IOHandler io_handler[SCENE_COUNT];
extern void reset_IOHandler(int sceneId);