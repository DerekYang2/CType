#pragma once
#include "globals.h"
#include "PopupHandler.h"
#include "Button.h"

class ToggleSpawn : public UIObject {
    Rectangle hitbox;
    PopupHandler* popup;
    Button* button;
    Texture* texture;
    float cx;
    
    public:
    ToggleSpawn(float c_x, float y, float height, PopupHandler* popup, Texture* texture);
    void update() override;
    void draw() override;
    void set_pos(float x2, float y2) override;
    bool needs_update();
    string get_selected();
};