#include "Scrollbar.h"

/**
 * Total height is mapped to bar.height, thus scale everything by bar.height/total_h
 * Theme colors:
 * normal: sub, hover: text, hold/press: main
*/

Scrollbar::Scrollbar(float x, float y, float w, float h, float screen_h, float total_h, list<UIObject*> children) : bar{ x, y, w, h }, screen_h(screen_h), total_h(total_h), children(children)
{
    // Screen height must not be greater than total height
    if (screen_h > total_h) screen_h = total_h;
    // The height of the bar is the screenheight scaled
    thumb_h = screen_h * (bar.height / total_h);
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
        // Get the offset at top_y position
        offset = offset_at(top_y);
    } else
    {
        Rectangle abs_bar = bar;
        abs_bar.x -= abs_bar.width;  // Make left aligned
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (hover)  // Pressed on thumb -> drag
            {
                drag = true;
                click_offset = mouse.y - thumb_rect().y;
            } else if (CheckCollisionPointRec(mouse, abs_bar))  // Pressed on bar -> set position
            {
                float top_y = mouse.y - thumb_h / 2;
                offset = offset_at(top_y);
            }
        }
    }
    // Update children shift
    for (UIObject* child : children)
    {
        child->set_offset(0, get_offset());
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
    DrawRectangleRoundedAlign(bar, 1, 4, theme.sub_alt, RIGHT, TOP);
    DrawRectangleRoundedAlign(thumb_rect(), 1, 4, thumb_col, LEFT, TOP);
}

void Scrollbar::shift(float dx, float dy)
{
    offset -= dy;
    offset = clamp(offset, -(total_h - screen_h), 0.f);
}

void Scrollbar::set_pos(float x2, float y2)
{
    bar.x = x2, bar.y = y2;
}

void Scrollbar::add_child(UIObject* child)
{
    children.push_back(child);
}

float Scrollbar::get_offset()
{
    return offset;
}

float Scrollbar::offset_at(float y)
{
    float real_offset = -(y - bar.y) / (bar.height / total_h);  // Unscale the scrollbar distance from scrollbar top
    return clamp(real_offset, -(total_h - screen_h), 0.f);
}

Rectangle Scrollbar::bounding_box()
{
    return bar;
}

// NOTE: original bar.x is right aligned, subtract bar.width from bar.x
Rectangle Scrollbar::thumb_rect()
{
    // Thumb rect subtracts offset because page movement is reverse of thumb movement
    return Rectangle(bar.x - bar.width, bar.y - offset * (bar.height / total_h), bar.width, thumb_h);
}

