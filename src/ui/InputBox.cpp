#include "InputBox.h"



InputBox::InputBox(float x, float y, float width, float height, string default_text, bool numeric) : default_text(default_text), text(default_text), numeric(numeric)
{
    rect = Rectangle(x, y, width, height);
    font_size = MeasureFontSize("|", 1000, rect.height);
    active = false;
}

void InputBox::set_range(int minv, int maxv)
{
    min_v = minv;
    max_v = maxv;
}

void InputBox::update()
{
    if (active)
    {
/*         // if IOHandler empty (inactive) 
        if (io_handler.input_chars.empty())
        {
            int key_pressed = GetKeyPressed();
            while (key_pressed)  // IF PRESSED
            {
                active = true;
                char char_pressed = convertKey(key_pressed);
                // shift the character pressed
                if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
                    char_pressed = shiftChar(char_pressed);
                if (char_pressed != 0)  // valid character
                {
                    text += char_pressed;
                } else if (key_pressed == KEY_BACKSPACE)
                {
                    if (!text.empty())
                        text.pop_back();
                }
                key_pressed = GetKeyPressed();
            }
        } else
        {
            for (char c : io_handler.input_chars)
            {
                if (c == 0)  // delete (backspace)
                {
                    if (!text.empty())
                    {
                        text.pop_back();
                    }
                } else
                {
                    text += c;
                }
            }
        } */
        if (!CheckCollisionPointRec(mouse, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            active = false;
        if (IsKeyPressed(KEY_ENTER))
            active = false;
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
        DrawRectangleLinesEx(rect, 2, theme.main);
    DrawTextAlign(text, rect.x, rect.y + rect.height, font_size, theme.main, LEFT, BOTTOM);
}

string InputBox::get_text()
{
    return text;
}