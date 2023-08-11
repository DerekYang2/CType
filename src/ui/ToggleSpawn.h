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
    float img_scale;
    float space_width;
    
    public:
    ToggleSpawn(float c_x, float y, float height, PopupHandler* popup, Texture* texture);
    void update() override;
    void draw() override;
    bool needs_update();
    string get_selected();
};