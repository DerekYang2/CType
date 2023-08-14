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
    text = toggle_text;
    font_size = MeasureFontSize(text, INT_MAX, h);
    space_width = MeasureTextEx("o", font_size).x;
    
    hitbox = { x, y, 0, h };
    hitbox.width = MeasureTextEx(text, font_size).x + space_width + texture->width * img_scale;
}

Toggle::Toggle(float x, float y, float h, bool initState, Texture* textureOn, Texture* textureOff)
{
    on = initState;
    texture = textureOn, texture_off = textureOff;
    img_scale = min(h / texture->width, h / texture->height);
    img_scale_off = min(h / texture_off->width, h / texture_off->height);
    text = "";
    hitbox = { x, y, h, h };
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
    if (hover) set_cursor(MOUSE_CURSOR_POINTING_HAND); //cursor_path = "link_cursor";
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
    if (texture == nullptr && texture_off == nullptr)  // Radio button toggle 
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
    } else  // Icon + text OR dual icon toggle
    {
        Color col;
        if (hover)
        {
            col = theme.text;
        } else if (on)
        {
            if (texture_off != nullptr)  // If dual icon toggle, view more as a button -> always theme.sub
                col = theme.sub;
            else  // single icon or text -> toggled on = theme.main
                col = theme.main;
        } else
        {
            col = theme.sub;
        }
        if (pressWatch.s() < TOGGLE_DELAY) col = theme.sub;  // blink off when click
        
        Vector2 corner, corner_off;
        // Calculate x and y position of texture
        if (texture_off == nullptr)  // Implies texture + text, don't center icon x
        {
            corner.x = hitbox.x;
            corner.y = hitbox.y + 0.5f * (hitbox.height - texture->height * img_scale);
        } else
        {
            corner = { hitbox.x + 0.5f * (hitbox.width - texture->width * img_scale), hitbox.y + 0.5f * (hitbox.height - texture->height * img_scale) };
            corner_off = {hitbox.x + 0.5f * (hitbox.width - texture_off->width * img_scale_off), hitbox.y + 0.5f * (hitbox.height - texture_off->height * img_scale_off)};
        }
        
        // Center the textures
        if (texture_off == nullptr || on)
        {
            DrawTextureEx(*texture, corner, 0, img_scale, col);
        } else
        {
            DrawTextureEx(*texture_off, corner_off, 0, img_scale_off, col);
        }
        if (!text.empty())
            DrawTextAlign(text, hitbox.x + texture->width * img_scale + space_width, hitbox.y + hitbox.height * 0.5f, font_size, col, LEFT, CENTER);
    }
}

float Toggle::get_width()
{
    return hitbox.width;
}

float Toggle::get_height()
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
