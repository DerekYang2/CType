#pragma once
#include "globals.h"
#include "Toggle.h"
#include "ToggleGroup.h"
#include "PopupHandler.h"

class SettingBar : public UIObject {
    float cx, cy, h, tot_width, space_width;
    float separator_x;
    unordered_map<string, Toggle*> toggle_map;
    ToggleGroup* toggle_group;
    PopupHandler* popup_handler;
    public:
    SettingBar(float center_x, float center_y, initializer_list<Toggle*> toggles, ToggleGroup* toggleGroup, PopupHandler* popupHandler);
    void update();
    void draw();
    bool needs_update();
    bool is_toggled(string str);
    string group_selected();
};

extern SettingBar* setting_bar;