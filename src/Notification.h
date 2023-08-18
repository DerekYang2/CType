#pragma once
#include "Utils.h"
#include "Stopwatch.h"

class Notification {
    Stopwatch stopwatch;
    float lifetime;
    string title = "";
    string text = "";
    Texture* texture = nullptr;
    Rectangle hitbox;
    float margin;
    float img_scale;
    float font_size;
    float text_height;
    float space_width;
    public:
    Notification(float margin, float font_size);
    void spawn_notification(string title, string text, float notification_time, Texture* texture = nullptr);
    void draw();
    void kill();
    bool is_alive();
};