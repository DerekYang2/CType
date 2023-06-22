#include "Toggle.h"

#define TOGGLE_DELAY 0.1
Toggle::Toggle(float x, float y, float w, float h, bool initState) 
{
    hitbox = { x, y, w, h };
    on = initState;
    text = "", texture = nullptr;
    hitbox.x -= h / 2, hitbox.width += h;

    rad = h / 2 - 3;
    if (on)
    {
        vx = 20;
        cx = hitbox.x + hitbox.width - hitbox.height / 2;
    } else
    {
        vx = -20;
        cx = hitbox.x + hitbox.height / 2;
    }
}

Toggle::Toggle(float x, float y, float h, bool initState, string toggle_text, string img_path) 
{
    on = initState;
    texture = &textureOf[img_path];
    img_scale = h / texture->height;
    text = " " + toggle_text;
    int maxFit = 2;
    font_size = maxFit;
    float padding = h / 15;

    while (true)  //find the max font size that fits
    {
        Vector2 textSize = MeasureTextEx(font, text.c_str(), maxFit, maxFit / font_spacing);
        textSize.y /= 1.5;
        if (textSize.y + 2 * padding <= h)
        {
            font_size = maxFit;
        } else
            break;
        maxFit++;
    }
    
    hitbox = { x, y, 0, h };
    hitbox.width = MeasureTextEx(font, text.c_str(), font_size, font_size / font_spacing).x + texture->width * img_scale;
}

void Toggle::set_x(float x)
{
    hitbox.x = x;
}

void Toggle::set_pos(float x, float y)
{
    hitbox.x = x, hitbox.y = y;
    if (text.empty())
    {
        if (on)
            cx = hitbox.x + hitbox.width - hitbox.height / 2;
        else
            cx = hitbox.x + hitbox.height / 2;
    }
}

void Toggle::flip()
{
    if (pressWatch.s() > TOGGLE_DELAY)
    {
        pressWatch.start();
        vx *= -1;
        on ^= true;
        pressed = true;
    }
}

void Toggle::set_state(bool state) {
    if (on != state) flip();
}

void Toggle::update()
{
    pressed = false;
    hover = CheckCollisionPointRec(mouse, hitbox);
    if (hover) SetMouseCursor(MOUSE_CURSOR_POINTING_HAND); //cursor_path = "link_cursor";
    if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        flip();
    if (text.empty())
    {
        cx += vx;
        if (cx < hitbox.x + hitbox.height / 2) cx = hitbox.x + hitbox.height / 2;
        if (cx > hitbox.x + hitbox.width - hitbox.height / 2) cx = hitbox.x + hitbox.width - hitbox.height / 2;
    }
}

void Toggle::draw()
{
    if (text.empty())
    {
        if (hover)
        {
            if (on)
                DrawRectangleRounded(hitbox, 1, 0, { 0, 160, 0, 255 });
            else
                DrawRectangleRounded(hitbox, 1, 0, { 160, 160, 160, 255 });
        } else
        {
            if (on)
                DrawRectangleRounded(hitbox, 1, 0, { 0, 200, 0, 255 });
            else
                DrawRectangleRounded(hitbox, 1, 0, { 200, 200, 200, 255 });
        }
        DrawCircle(cx, hitbox.y + hitbox.height / 2, rad + hover, WHITE);
    } else
    {
        Color col = (hover || on) ? theme.main : theme.sub;
        if (pressWatch.s() < TOGGLE_DELAY) col = theme.sub;  // blink off when click
        DrawTextureEx(*texture, Vector2(hitbox.x, hitbox.y), 0, img_scale, col);
        DrawTextAlign(text, hitbox.x + texture->width * img_scale, hitbox.y + hitbox.height * 0.5f, font_size, col, LEFT, CENTER);
    }
}

float Toggle::width()
{
    return hitbox.width;
}

float Toggle::height()
{
    return hitbox.height;
}

string Toggle::get_text()
{
    return text;
}

bool Toggle::toggled()
{
    return on;
}

bool Toggle::was_pressed()
{
    return pressed;
}
