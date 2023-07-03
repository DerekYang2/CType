#include "InputBox.h"



InputBox::InputBox(float x, float y, float width, float height, string default_text, bool numeric) : default_text(default_text), text(default_text), numeric(numeric)
{
    const float padding = max(width, height) * 0.05f * 0.5f;
    cout << padding << endl;
    rect = Rectangle(x, y, width, height);
    outer_rect = Rectangle(x - padding, y - padding, width + 2 * padding, height + 2 * padding);
    font_size = MeasureFontSize("|", 1000, rect.height);
    active = false;
    active_frames = 0;
    invalid_frames = 0;
    text_idx = text.size() - 1;
    left_frames = 0;
    right_frames = 0;
}

void InputBox::set_range(int minv, int maxv)
{
    min_v = minv;
    max_v = maxv;
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

        left_frames = IsKeyDown(KEY_LEFT) ? left_frames + 1 : 0;
        right_frames = IsKeyDown(KEY_RIGHT) ? right_frames + 1 : 0;
        
        // HANDlE LEFT OR RIGHT KEY PRESS 
        if ((IsKeyPressed(KEY_LEFT) || left_frames > 30) && text_idx >= 0)
        {
            active_frames = 30;
            text_idx--;
        }

        if ((IsKeyPressed(KEY_RIGHT) || right_frames > 30) && text_idx < (int)text.size() - 1)
        {
            active_frames = 30;
            text_idx++;
        }
        
        active_frames--;
        if (active_frames < -60) active_frames = -1;  // prevent overflow
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
    if (active)
    {
        float offset_x = 0;
        if (invalid_frames > 0)
            offset_x = 0.2f * (15 - invalid_frames) * sin(2 * (15 - invalid_frames)) + sin(GetTime() * 90);
        rect.x += offset_x, outer_rect.x += offset_x;
        
        DrawRectangleRounded(outer_rect, 0.3f, 7, theme.background_shade);
        // DRAW OUTLINE
        DrawRectangleRoundedLines(outer_rect, 0.3f, 7, 2, invalid_frames > 0 ? theme.error : theme.main);
        // DRAW CURSOR
        if (active_frames > 0 || (-active_frames/30) & 1)  // if active or Inactive, blink half of the time
        {
            float cursor_x = rect.x + MeasureTextEx(substrI(text, 0, text_idx), font_size).x;
            DrawRectangleRoundedAlign(Rectangle(cursor_x, rect.y + rect.height, 3, rect.height), 0.8f, 7, theme.caret, LEFT, BOTTOM);
        }
        // DRAW TEXT
        DrawTextAlign(text, rect.x, rect.y + rect.height, font_size, theme.main, LEFT, BOTTOM);
        rect.x -= offset_x, outer_rect.x -= offset_x;  // unoffset

        float x_sum = 0;
        // test position 
        for (int i = 0; i < text.size(); i++)
        {
            string char_str = string(1, text[i]);
            float cur_w = MeasureTextEx(font, char_str.c_str(), font_size, font_size / font_spacing).x;
            DrawRectangleAlign({ rect.x + x_sum, rect.y, cur_w, 10 }, (i&1)?BLUE:RED, LEFT, BOTTOM);
            x_sum += cur_w;
        }
    } else
    {
        DrawRectangleRounded(outer_rect, 0.3f, 7, theme.background_shade);
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
            string after_text = (text_idx + 1 < text.size()) ? text.substr(text_idx + 1) : "";  
            text = substrI(text, 0, text_idx) + c + after_text;
            text_idx++;
            active_frames = 30;
        } else
        {
            invalid_frames = 15;
        }
    }
}

void InputBox::pop_char()
{
    if (active && text_idx >= 0)  // text idx can become -1
    {
        active_frames = 30;
        string after_text = (text_idx + 1 < text.size()) ? text.substr(text_idx + 1) : "";
        text = substrI(text, 0, text_idx - 1) + after_text;
        text_idx--;
    }
}

void InputBox::set_IOHandler(int sceneId)
{
    input_boxes[sceneId].push_back(this);
}