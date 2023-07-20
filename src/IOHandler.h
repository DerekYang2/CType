#pragma once
#include "StatusHandling.h"
#include "WpmLogger.h"
#include "InputBox.h"
#define inactive_time 30
#define blink_time 30
struct IOHandler {
    int active_frames = 0;
    int back_frames = 0;
    bool handled_press[CHAR_MAX + 1];
    function<void(char)> add_function;
    function<void()> back_function;
    IOHandler();
    IOHandler(function<void(char)> add_function, function<void()> back_function);
    void update();
    bool active_cursor();
};

extern IOHandler io_handler[SCENE_COUNT];
extern void reset_IOHandler(int sceneId);