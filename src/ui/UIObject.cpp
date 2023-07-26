#include "UIObject.h"

const float HINT_ALPHA = 0.75f;

UIObject::UIObject() {}

UIObject::~UIObject() {}

void UIObject::draw() {}

void UIObject::draw_hint() {}

void UIObject::update() {}

void UIObject::set_pos(float x2, float y2)
{
    x = x2;  y = y2;
}