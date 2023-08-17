#include "VerticalGroup.h"

VerticalGroup::VerticalGroup() {}

VerticalGroup::VerticalGroup(float cx, float top_y, float padding, std::vector<UIObject*> objects, bool show_rect) : objects(objects), cx(cx), top_y(top_y), padding(padding), show_rect(show_rect)
{
    max_width = 0;
    tot_height = 0;
    for (int i = 0; i < objects.size(); i++)
    {
        max_width = std::max(max_width, objects[i]->get_width());
        tot_height += objects[i]->get_height();
    }
    tot_height += (objects.size() + 1) * padding;

    float y_pos = top_y + padding;
    for (UIObject* obj : objects)
    {
        obj->set_pos(cx - obj->get_width() * 0.5f, y_pos);
        y_pos += obj->get_height() + padding;
    }
}

void VerticalGroup::update()
{
    for (UIObject* obj : objects)
    {
        obj->update();
    }
}

void VerticalGroup::draw()
{
    if (show_rect)
        DrawRectangleRoundedAlign(cx, top_y, max_width + padding, tot_height, 0.3f, roundedSegments(max_width + padding), theme.sub_alt, CENTER, TOP);
    for (UIObject* obj : objects)
    {
        obj->draw();
    }
}

void VerticalGroup::draw_hint()
{
    for (UIObject* obj : objects)
    {
        obj->draw_hint();
    }
}

void VerticalGroup::set_pos(float x, float y)
{
    cx = x, top_y = y;
    float y_pos = top_y + padding;
    for (UIObject* obj : objects)
    {
        obj->set_pos(cx - obj->get_width() * 0.5f, y_pos);
        y_pos += obj->get_height() + padding;
    }
}

float VerticalGroup::get_width()
{
    return max_width;
}

float VerticalGroup::get_height()
{
    return tot_height;
}
