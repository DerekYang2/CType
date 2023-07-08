#include "Textbox.h"

Textbox::Textbox(float x, float y, float width, float height, string text_str, float fontSize, Color col, bool wrapping) : text(text_str), font_size(fontSize), color(col), wrapping(wrapping)
{
    og_text = text;
    rect.x = x;
    rect.y = y;
    if (wrapping)
    {
        text = fold(text, width);
        Vector2 dimension = MeasureTextEx(text, font_size);
        rect.width = width;
        rect.height = dimension.y;
    } else
    {
        font_size = MeasureFontSize(text, width, height);
        Vector2 dimension = MeasureTextEx(text, font_size);
        rect.width = dimension.x;
        rect.height = dimension.y;
    }
}

Textbox::Textbox(float x, float y, float width, string text, Color col) : text(text), color(col), wrapping(false)
{
    og_text = text;
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

void Textbox::set_var_str(vector<string> strs)
{
    text = og_text;  // reset to original
    // search for %s and replace with str
    for (auto& str : strs)
    {
        if (!replace(text, "%s", str))  // if replacement failed, break
            break;
    }
    
    if (wrapping)  // fold text and resize height
    {
        text = fold(text, rect.width);
        Vector2 dimension = MeasureTextEx(text, font_size);
        rect.height = dimension.y;
    } else   // fold text and resize 
    {
        font_size = MeasureFontSize(text, rect.width, rect.height);
        Vector2 dimension = MeasureTextEx(text, font_size);
        rect.width = dimension.x;
        rect.height = dimension.y;
    }
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
