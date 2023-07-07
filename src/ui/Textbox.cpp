#include "Textbox.h"

Textbox::Textbox(float x, float y, float width, float height, string text_str, float fontSize, Color col, bool wrapping) : text(text_str), font_size(fontSize), color(col), wrapping(wrapping)
{
    rect.x = x;
    rect.y = y;
    rect.width = width;
    rect.height = height;

    if (wrapping)
    {
        text = fold(text, width);
        rect.height = MeasureTextEx(text, font_size).y;
    } else
    {
        font_size = MeasureFontSize(text, width, height);
    }
}

Textbox::Textbox(float x, float y, float width, string text, Color col) : text(text), color(col)
{
    rect.x = x;
    rect.y = y;
    rect.width = width;
    font_size = MeasureFontSize(text, width);
    rect.height = MeasureTextEx(text, font_size).y;
}

void Textbox::draw()
{
    DrawTextAlign(text, rect.x, rect.y, font_size, color);
}

void Textbox::set_pos(float x, float y)
{
    rect.x = x;
    rect.y = y;
}

Rectangle Textbox::get_rect()
{
    return rect;
}

float Textbox::get_width()
{
    return rect.width;
}

float Textbox::get_height()
{
    return rect.height;
}

string Textbox::fold(string str, float max_width)
{
    int prev_space = 0;
    int start_pos = 0;
    string fold_str = "";
    for (int i = 0; i < str.length(); i++)
    {
        string char_str(1, str[i]);
        // check remaining 
        float remain = max_width - MeasureTextEx(substrI(str, start_pos, i), font_size).x;
        if (remain < 0)
        {
            fold_str += substrI(str, start_pos, prev_space);
            fold_str += "\n";
            start_pos = prev_space + 1;
            i = start_pos;
        }
        if (char_str == " ")
            prev_space = i;
    }
    // final line
    fold_str += substrE(str, start_pos, str.length());
    return fold_str;
}
