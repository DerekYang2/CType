#include "Scrollbar.h"

/**
 * Total height is mapped to rect.height, thus scale everything by rect.height/total_h
 * Theme colors:
 * normal: sub, hover: text, hold/press: main
*/

Scrollbar::Scrollbar(float x, float y, float w, float h, float screen_h, float total_h) : rect{ x, y, w, h }, screen_h(screen_h), total_h(total_h)
{
    // The height of the bar is the screenheight scaled
    thumb_h = screen_h * (rect.height / total_h);
}
    
void Scrollbar::update()
{
    hover = CheckCollisionPointRec(mouse, thumb_rect());
    shift(0, -abs(GetMouseWheelMove()) * GetMouseWheelMove() * 40);
    if (drag)
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            drag = false;
        float top_y = mouse.y - click_offset;  // Top of the thumb rect
        offset = -(top_y - rect.y) / (rect.height / total_h);  // Unscale the scrollbar distance from scrollbar top
        offset = clamp(offset, -(total_h - screen_h), 0.f);
    } else
    {
        if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            drag = true;
            click_offset = mouse.y - thumb_rect().y;
        }
    }
}

void Scrollbar::draw()
{
    Color thumb_col = theme.sub;
    if (drag)
    {
        thumb_col = theme.main;
    } else if (hover)
    {
        thumb_col = theme.text;
    }
    DrawRectangleRoundedAlign(rect, 1, 4, theme.sub_alt, RIGHT, TOP);
    DrawRectangleRoundedAlign(thumb_rect(), 1, 4, thumb_col, LEFT, TOP);
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

// NOTE: original rectangle.x is right aligned, subtract rect.width from x
Rectangle Scrollbar::thumb_rect()
{
    // Thumb rect subtracts offset because page movement is reverse of thumb movement
    return Rectangle(rect.x - rect.width, rect.y - offset * (rect.height / total_h), rect.width, thumb_h);
}

