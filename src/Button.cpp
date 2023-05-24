#include "Button.h"

Button::Button(float x, float y, float w, float h, string img_path, Color tint, std::function<void()> f) : img_path(img_path), default_color(tint), triggerFunc(f)
{
    message = "";
    hitbox = Rectangle(x, y, w, h);
    hover_color = add_rgb(main_color, 5), click_color = add_rgb(main_color, 10);
}

Button::Button(float x, float y, float w, float h, string text, std::function<void()> f): triggerFunc(f)
{
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

    hover_color = add_rgb(main_color, 5), click_color = add_rgb(main_color, 10);
}

void Button::attachDraw(std::function<void(Rectangle)> f) { drawFunc = f; }

void Button::setPos(float x, float y) 
{
    hitbox.x = x, hitbox.y = y;
}

void Button::update()
{
    hover = CheckCollisionPointRec(mouse, hitbox);
    if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && pressWatch.s() > delay)
    {
        pressWatch.start();
        triggerFunc();
    }
}

void Button::setColor(Color defaultCol, Color hoverCol, Color clickCol)
{
    default_color = defaultCol, hover_color = hoverCol, click_color = clickCol;
}

void Button::setStroke(int strokeWidth)
{
    stroke = strokeWidth;
}

void Button::draw()
{
    if (hover)
    {
        //DrawRectangleRec(hitbox, hover_color);
        DrawRectangleRounded(hitbox, 0.2, 5, hover_color);
    } else if (img_path.empty())
    {
        // DrawRectangleRec(hitbox, default_color);
        DrawRectangleRounded(hitbox, 0.2, 5, default_color);
    }
    if (pressWatch.s() <= delay)
    {
        if (pressWatch.s() <= 0.05)
            DrawRectangleRounded(hitbox, 0.2, 5, click_color);
        else
            DrawRectangleRounded(hitbox, 0.2, 5, hover_color);

        float xdist = hitbox.width / 2 * (1 - (float)pressWatch.s() / delay);
        float ydist = hitbox.height / 2 * (1 - (float)pressWatch.s() / delay);

        DrawLineEx({ hitbox.x, hitbox.y }, { hitbox.x + xdist, hitbox.y }, 2 * stroke, stroke_color);
        DrawLineEx({ hitbox.x + hitbox.width - xdist, hitbox.y }, { hitbox.x + hitbox.width, hitbox.y }, 2 * stroke, stroke_color);
        DrawLineEx({ hitbox.x, hitbox.y + hitbox.height }, { hitbox.x + xdist, hitbox.y + hitbox.height }, 2 * stroke, stroke_color);
        DrawLineEx({ hitbox.x + hitbox.width - xdist, hitbox.y + hitbox.height }, { hitbox.x + hitbox.width, hitbox.y + hitbox.height }, 2 * stroke, stroke_color);
        DrawLineEx({ hitbox.x, hitbox.y }, { hitbox.x, hitbox.y + ydist }, 2 * stroke, stroke_color);
        DrawLineEx({ hitbox.x, hitbox.y + hitbox.height }, { hitbox.x, hitbox.y + hitbox.height - ydist }, 2 * stroke, stroke_color);
        DrawLineEx({ hitbox.x + hitbox.width, hitbox.y }, { hitbox.x + hitbox.width, hitbox.y + ydist }, 2 * stroke, stroke_color);
        DrawLineEx({ hitbox.x + hitbox.width, hitbox.y + hitbox.height }, { hitbox.x + hitbox.width, hitbox.y + hitbox.height - ydist }, 2 * stroke, stroke_color);
    }
    if (!message.empty())
    {
        DrawTextAlign(message, hitbox.x + (hitbox.width - msg_width) / 2, hitbox.y + (hitbox.height-msg_height)/2, fontSize, stroke_color);
    }
    if (!img_path.empty())
    {
        if (drawFunc != NULL)
            drawFunc(hitbox);
        // shrink image a little
        float padding = hitbox.width / 15;
        DrawTexturePro(textureOf[img_path], Rectangle(0, 0, textureOf[img_path].width, (flipped?-1:1) * textureOf[img_path].height), Rectangle(hitbox.x + padding, hitbox.y + padding, hitbox.width - 2 * padding,  hitbox.height - 2 * padding), { 0,0 }, 0, default_color); //for now

        //DrawTexturePro(img_path, Rectangle(hitbox.x + padding, hitbox.y + padding, hitbox.width - 2 * padding, (flipped ? -1 : 1) * (hitbox.height - 2 * padding)), default_color);
    }
}


