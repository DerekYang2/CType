#include "InputBox.h"



InputBox::InputBox(float x, float y, float width, float height, string default_text, bool numeric) : default_text(default_text), text(default_text), numeric(numeric)
{
    rect = Rectangle(x, y, width, height);
    font_size = MeasureFontSize("|", 1000, rect.height);
    active = false;
    cursor_x = rect.x + MeasureTextEx(default_text, font_size).x;
    active_frames = 0;
    invalid_frames = 0;
}

void InputBox::set_range(int minv, int maxv)
{
    min_v = minv;
    max_v = maxv;
    cursor_x = x;
}

void InputBox::update()
{
    if (invalid_frames > 0) invalid_frames--;
    if (active)
    {
        if (!CheckCollisionPointRec(mouse, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            active = false;
        if (IsKeyPressed(KEY_ENTER))
            active = false;
        if (active_frames > 0) 
            active_frames--;
    } else
    {
        if (CheckCollisionPointRec(mouse, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            active = true;
        }
    }
}

void InputBox::draw()
{
    DrawRectangleRec(rect, theme.background_shade);
    if (active)
    {
        float offset_x = 0;
        if (invalid_frames > 0)
            offset_x = 0.2f * (15 - invalid_frames) * sin(2 * (15 - invalid_frames)) + sin(GetTime() * 90);
        rect.x += offset_x;
        
        // DRAW OUTLINE
        DrawRectangleLinesEx(rect, 2, invalid_frames > 0 ? theme.error : theme.main);
        // DRAW CURSOR
        if (active_frames > 0 || (int)(GetTime() * 1.f / 0.5f) & 1)  // if active or Inactive, blink half of the time
        {
            cursor_x = rect.x + MeasureTextEx(text, font_size).x;
            DrawRectangleRoundedAlign(Rectangle(cursor_x, rect.y + rect.height, 3, rect.height), 0.8f, 7, theme.caret, LEFT, BOTTOM);
        }
        // DRAW TEXT
        DrawTextAlign(text, rect.x, rect.y + rect.height, font_size, theme.main, LEFT, BOTTOM);
        rect.x -= offset_x;  // unoffset
    } else
    {
        // DRAW TEXT
        DrawTextAlign(text, rect.x, rect.y + rect.height, font_size, theme.main, LEFT, BOTTOM);
    }
}

string InputBox::get_text()
{
    return text;
}

void InputBox::push_char(char c)
{
    if (active)
    {
        if (MeasureTextEx(text + c, font_size).x <= rect.width)
        {
            text += c;
            active_frames = 60;
        } else
        {
            invalid_frames = 15;
        }
    }
}

void InputBox::pop_char()
{
    if (active && !text.empty())
    {
        active_frames = 60;
        text.pop_back();
    }
}

void InputBox::set_IOHandler(int sceneId)
{
    input_boxes[sceneId].push_back(this);
}