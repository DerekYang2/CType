#include "HorizontalGroup.h"

HorizontalGroup::HorizontalGroup() {}

HorizontalGroup::HorizontalGroup(float cx, float cy, float padding, vector<UIObject*> objects, bool show_rect): objects(objects), cx(cx), cy(cy), padding(padding), show_rect(show_rect)
{
    tot_width = 0, max_height = 0;
    for (int i = 0; i < objects.size(); i++)
    {
        tot_width += objects[i]->get_width();
        max_height = max(max_height, objects[i]->get_height());
    }
    tot_width += (objects.size() + 1) * padding;

    float x_pos = cx - tot_width * 0.5f + padding;
    for (UIObject* obj : objects)
    {
        obj->set_pos(x_pos, cy - obj->get_height() * 0.5f);
        x_pos += obj->get_width() + padding;
    }
}

void HorizontalGroup::update()
{
    for (UIObject* obj : objects)
    {
        obj->update();
    }
}

void HorizontalGroup::draw()
{
    DrawRectangleRoundedAlign(cx, cy, tot_width, max_height + padding, 0.3f, roundedSegments(max_height + padding), theme.sub_alt, CENTER, CENTER);
    for (UIObject* obj : objects)
    {
        obj->draw();
    }
}

void HorizontalGroup::draw_hint()
{
    for (UIObject* obj : objects)
    {
        obj->draw_hint();
    }
}

void HorizontalGroup::set_pos(float x, float y)
{
    cx = x, cy = y;
    float x_pos = cx - tot_width * 0.5f + padding;
    for (UIObject* obj : objects)
    {
        obj->set_pos(x_pos, cy - obj->get_height() * 0.5f);
        x_pos += obj->get_width() + padding;
    }
}

float HorizontalGroup::get_width()
{
    return tot_width;
}

float HorizontalGroup::get_height()
{
    return max_height;
}
