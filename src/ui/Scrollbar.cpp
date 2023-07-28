#include "Scrollbar.h"

/**
 * total height is mapped to rect.height, thus scale everything by rect.height/total_h
*/

Scrollbar::Scrollbar(float x, float y, float w, float h, float screen_h, float total_h) : rect{ x, y, w, h }, screen_h(screen_h), total_h(total_h)
{
    // The height of the bar is the screenheight scaled
    thumb_h = screen_h * (rect.height / total_h);
}
    
void Scrollbar::update()
{
    shift(0, -abs(GetMouseWheelMove()) * GetMouseWheelMove() * 40);
}

void Scrollbar::draw()
{
    DrawRectangleRoundedAlign(rect, 1, 4, theme.sub_alt, RIGHT, TOP);
    DrawRectangleRoundedAlign(thumb_rect(), 1, 4, theme.sub, RIGHT, TOP);
}

void Scrollbar::shift(float dx, float dy)
{
    offset -= dy;
    offset = clamp(offset, -(total_h - screen_h), 0.f);
}

void Scrollbar::set_pos(float x2, float y2)
{
    rect.x = x2, rect.y = y2;
}

float Scrollbar::get_offset()
{
    return offset;
}

Rectangle Scrollbar::bounding_box()
{
    return rect;
}

Rectangle Scrollbar::thumb_rect()
{
    return Rectangle(rect.x, rect.y - offset * (rect.height / total_h), rect.width, thumb_h);
}

