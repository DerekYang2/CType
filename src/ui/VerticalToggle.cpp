#include "VerticalToggle.h"

/**
 * TOOD: Crop when toggle width is too wide
*/

VerticalToggle::VerticalToggle(float x, float y, float w, float h, int rows, vector<string> options, string init_option) : ToggleGroup(x, y, h, 0, { "anything" }, true)
{
    tot_width = w;
    text.clear();
    text = deque<string>(options.begin(), options.end());
    for (int i = 0; i < text.size(); i++)
    {
        if (text[i] == init_option) selected = i;
    }
    
    font_size = MeasureFontSize("A", INT_MAX, h / 1.5f);
    padding = h * 1.1f;
    float rect_w = tot_width - 2 * padding;

    total_hitbox = Rectangle(corner.x, corner.y, tot_width, rows * h + 2 * padding);
    
    hitbox.clear();
    for (int i = 0; i < text.size(); i++)
        hitbox.push_back({ 0, 0, rect_w, h });
    
    for (int i = 0; i < hitbox.size(); i++)
    {
        hitbox[i].x = corner.x + padding;
        hitbox[i].y = corner.y + padding + i * h;
    }
    local_init_corner = {hitbox[0].x, hitbox[0].y};
    
    // Init scrollbar
    visible_h = rows * h;  // Height of visible scrollbar and toggles
    const float bar_w = 10;
    scrollbar = new Scrollbar(corner.x + tot_width - bar_w / 3, corner.y + padding, bar_w, visible_h, visible_h, hitbox.size() * h);
}

void VerticalToggle::update()
{
    // update scrollbar 
    if (CheckCollisionPointRec(mouse, RectangleIntersection(total_bounds, total_hitbox)))
    {
        scrollbar->update();
    }
    set_local_offset(0, scrollbar->get_offset());
    // Update bound for toggles
    Rectangle local_bounds = Rectangle(corner.x + padding, corner.y + padding, tot_width - 2 * padding, visible_h);
    bounds = RectangleIntersection(total_bounds, local_bounds);
    ToggleGroup::update();
}

void VerticalToggle::draw()
{
    //DrawRectangleRoundedLinesAlign(total_hitbox, 0.3f, roundedSegments(total_hitbox.height), 2.f, theme.text);
    DrawRectangleRec(total_hitbox, theme.background);
    DrawRectangleRec(bounds, theme.sub_alt);
    for (int i = 0; i < hitbox.size(); i++)
    {
        if (!CheckCollisionRecs(hitbox[i], bounds)) continue;
        Color text_col;
        if (i == selected || hover[i])
        {
            text_col = theme.background;
            DrawRectangleRec(hitbox[i], theme.text);
        } else
        {
            text_col = theme.text;
        }
        // Draw selection indicator
        if (i == selected)
        {
            float radius = hitbox[i].height;
            DrawRectangleRoundedAlign(hitbox[i].x + 0.5f * radius, hitbox[i].y + 0.5f * hitbox[i].height, radius * 0.5f, radius * 0.5f, 0.4f, 4, theme.main, CENTER, CENTER);
        }
        // Shift text over by hitbox height
        DrawTextAlign(text[i], hitbox[i].x + hitbox[i].height, hitbox[i].y + hitbox[i].height * 0.5f, font_size, text_col, LEFT, CENTER);
    }

    // Draw top/bottom paddings
    DrawRectangle(corner.x + padding, corner.y, tot_width - 2 * padding, padding, theme.background);
    DrawRectangle(corner.x + padding, corner.y + padding + visible_h, tot_width - 2 * padding, padding, theme.background);
    scrollbar->draw();
}

void VerticalToggle::set_bounds(Rectangle boundRect)
{
    total_bounds = boundRect;
    // Set bounds for the toggles
    bounds = RectangleIntersection(total_bounds, Rectangle(corner.x + padding, corner.y + padding, tot_width - 2 * padding, visible_h));
}

void VerticalToggle::set_pos(float x2, float y2)
{
    y2 -= padding;  // Remove extra padding
    float shift_x = x2 - corner.x, shift_y = y2 - corner.y;
    corner.x += shift_x, corner.y += shift_y;
    for (int i = 0; i < hitbox.size(); i++)
        hitbox[i].x += shift_x, hitbox[i].y += shift_y;
    local_init_corner.x += shift_x, local_init_corner.y += shift_y;
    corner_init = { x2, y2 };
    
    total_hitbox = Rectangle(corner.x, corner.y, tot_width, total_hitbox.height);
    const float bar_w = 10;
    scrollbar->set_pos(corner.x + tot_width - bar_w / 3, corner.y + padding);
    set_local_offset(0, scrollbar->get_offset());
}

void VerticalToggle::set_offset(float dx, float dy)
{
    float shift_x = (corner_init.x + dx) - corner.x, shift_y = (corner_init.y + dy) - corner.y;
    corner.x += shift_x, corner.y += shift_y;
    for (int i = 0; i < hitbox.size(); i++)
        hitbox[i].x += shift_x, hitbox[i].y += shift_y;
    local_init_corner.x += shift_x, local_init_corner.y += shift_y;
    // Set positions of objects
    total_hitbox = Rectangle(corner.x, corner.y, tot_width, total_hitbox.height);
    const float bar_w = 10;
    scrollbar->set_pos(corner.x + tot_width - bar_w / 3, corner.y + padding);
}

void VerticalToggle::set_local_offset(float dx, float dy)
{
    float shift_x = (local_init_corner.x + dx) - hitbox[0].x, shift_y = (local_init_corner.y + dy) - hitbox[0].y;
    for (int i = 0; i < hitbox.size(); i++)
        hitbox[i].x += shift_x, hitbox[i].y += shift_y;
}

Rectangle VerticalToggle::bounding_box()
{
    return total_hitbox;
}

float VerticalToggle::get_width()
{
    return tot_width;
}

float VerticalToggle::get_height()
{
    return total_hitbox.height - 2*padding;
}

string VerticalToggle::get_selected()
{
    return trim(text[selected]);
}

bool VerticalToggle::was_pressed()
{
    return pressed;
}
