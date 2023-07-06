#include "Button.h"

Button::Button(float x, float y, float w, float h, Texture *texture_pointer, std::function<void()> f) : triggerFunc(f)
{
    message = "";
    texture = texture_pointer;
    hitbox = Rectangle(x, y, w, h);
}

Button::Button(float x, float y, float w, float h, string text, std::function<void()> f): triggerFunc(f)
{
    texture = nullptr;
    message = text;
    hitbox = Rectangle(x, y, w, h);
    int maxFit = 2;
    float padding = w / 15;
    while (true)  //find the max font size that fits
    {
        Vector2 textSize = MeasureTextEx(font, text.c_str(), maxFit, maxFit / font_spacing);
        textSize.y /= 1.5;
        if (textSize.x + 2 * padding <= hitbox.width && textSize.y + 2 * padding <= hitbox.height)
        {
            fontSize = maxFit;
        } else
            break;
        maxFit++;
    }
    Vector2 textSize = MeasureTextEx(font, message.c_str(), fontSize, fontSize / font_spacing);
    msg_width = textSize.x, msg_height = textSize.y * 1.1;
}

void Button::attachDraw(std::function<void(Rectangle)> f) { drawFunc = f; }

void Button::set_pos(float x, float y) 
{
    hitbox.x = x, hitbox.y = y;
}

void Button::update()
{
    hover = CheckCollisionPointRec(mouse, hitbox);
    if (hover) SetMouseCursor(MOUSE_CURSOR_POINTING_HAND); //cursor_path = "link_cursor";
    if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && pressWatch.s() > delay)
    {
        pressWatch.start();
        triggerFunc();
    }
}

void Button::setStroke(int strokeWidth)
{
    stroke = strokeWidth;
}

void Button::draw()
{

    if (texture == nullptr)
    {
        Color col = hover ? theme.background_shade : theme.main;  // text color
        if (pressWatch.s() <= 0.05)
            col = theme.main;

        DrawRectangleRounded(hitbox, 0.2f, 7, (col == theme.main) ? theme.background_shade : theme.main);  // opposite color
        DrawTextAlign(message, hitbox.x + (hitbox.width) * 0.5f, hitbox.y + (hitbox.height) * 0.5f, fontSize, col, CENTER, CENTER);
    } else
    {
        Color col = hover ? theme.main : theme.sub;

        if (pressWatch.s() <= 0.05)
            col = theme.sub;
        if (drawFunc != NULL)
            drawFunc(hitbox);
        // shrink image a little
        float padding = hitbox.width / 15;
        DrawTexturePro(*texture, Rectangle(0, 0, texture->width, (flipped?-1:1) * texture->height), Rectangle(hitbox.x + padding, hitbox.y + padding, hitbox.width - 2 * padding,  hitbox.height - 2 * padding), { 0,0 }, 0, col); //for now
        //DrawTexturePro(img_path, Rectangle(hitbox.x + padding, hitbox.y + padding, hitbox.width - 2 * padding, (flipped ? -1 : 1) * (hitbox.height - 2 * padding)), default_color);
    }

}


