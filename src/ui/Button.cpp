#include "Button.h"

Button::Button(float x, float y, float h, Texture *texture_pointer, std::function<void()> f, string hint) : triggerFunc(f), height(h), hint(hint)
{
    reset();
    message = "";
    texture = texture_pointer;
    img_scale = height / texture->height;
    hitbox = { x, y, texture->width * img_scale, height };
    fontSize = MeasureFontSize(hint, INT_MAX, hitbox.height * 0.8f);
}

Button::Button(float x, float y, float w, float h, string text, std::function<void()> f): triggerFunc(f), height(h)
{
    reset();
    texture = nullptr;
    message = text;
    hitbox = Rectangle(x, y, w, h);
    fontSize = MeasureFontSize(text, hitbox.width * 0.8f, hitbox.height * 0.8f);
}

Button::Button(float x, float y, float h, string text, std::function<void()> f) : triggerFunc(f), height(h)
{
    reset();
    texture = nullptr;
    message = text;
    fontSize = MeasureFontSize(text, INT_MAX, h / 1.2f);
    Vector2 text_wh = MeasureTextEx(message, fontSize);
    hitbox = Rectangle(x, y, text_wh.x * 1.2, text_wh.y * 1.2);
}

Button::Button(float x, float y, float h, string text, Texture* texture_pointer, std::function<void()> f) : triggerFunc(f), height(h)
{
    reset();
    texture = texture_pointer;
    img_scale = height / texture->height;
    message = text;
    fontSize = MeasureFontSize(text, INT_MAX, height);
    space_width = MeasureTextEx("o", fontSize).x;

    hitbox = { x, y, 0, height };
    hitbox.width = MeasureTextEx(message, fontSize).x + space_width + texture->width * img_scale;
}

void Button::reset()
{
    hover = false;
    pressWatch = Stopwatch();
    hint_alpha = 0;
}

void Button::attachDraw(std::function<void(Rectangle)> f) { drawFunc = f; }

void Button::attach_trigger(std::function<void()> f)
{
    triggerFunc = f;
}

void Button::set_pos(float x, float y) 
{
    hitbox.x = x, hitbox.y = y;
}

void Button::set_text(string text)
{
    message = text;
    hitbox.width = MeasureTextEx(message, fontSize).x + space_width + texture->width * img_scale;
}

void Button::update()
{
    hover = CheckCollisionPointRec(mouse, hitbox);
    if (hover)
    {
        set_cursor(MOUSE_CURSOR_POINTING_HAND); //cursor_path = "link_cursor";
        hint_alpha = clamp(hint_alpha + 0.1f, 0.f, HINT_ALPHA);
    } else
    {
        hint_alpha = clamp(hint_alpha - 0.1f, 0.f, HINT_ALPHA);
    }
    if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && pressWatch.s() > delay)
    {
        pressWatch.start();
        if (triggerFunc != NULL)
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
        Color col = hover ? theme.sub_alt : theme.text;  // text color
        if (pressWatch.s() <= 0.05)
            col = theme.text;
        DrawRectangleRounded(hitbox, 0.3f, roundedSegments(hitbox.height), (col == theme.text) ? theme.sub_alt : theme.text);  // opposite color
        DrawTextAlign(message, hitbox.x + (hitbox.width) * 0.5f, hitbox.y + (hitbox.height) * 0.5f, fontSize, col, CENTER, CENTER);
    } else
    {
        Color col = hover ? theme.text : theme.sub;

        if (pressWatch.s() <= 0.05)
            col = theme.sub;
        if (drawFunc != NULL)
            drawFunc(hitbox);
        // float padding = 0;
        if (message.empty())  // Texture
        {
            DrawTextureEx(*texture, { hitbox.x, hitbox.y }, 0, img_scale, col);
            // DrawTexturePro(*texture, Rectangle(0, 0, texture->width, (flipped ? -1 : 1) * texture->height), Rectangle(hitbox.x + padding, hitbox.y + padding, hitbox.width - 2 * padding, hitbox.height - 2 * padding), { 0,0 }, 0, col); //for now
        } else  // Texture and text
        {
            Vector2 corner { hitbox.x, hitbox.y + 0.5f * (hitbox.height - texture->height * img_scale) };
            DrawTextureEx(*texture, corner, 0, img_scale, col);
            DrawTextAlign(message, hitbox.x + texture->width * img_scale + space_width, hitbox.y + (hitbox.height) * 0.5f, fontSize, col, LEFT, CENTER);
        }
        //DrawTexturePro(img_path, Rectangle(hitbox.x + padding, hitbox.y + padding, hitbox.width - 2 * padding, (flipped ? -1 : 1) * (hitbox.height - 2 * padding)), default_color);
    }

}

// TODO: shift hint up (for button and toggle group)
void Button::draw_hint()
{
    if (hint.empty()) return;
    const float padding = MeasureTextEx("o", fontSize).x;
    const Vector2 text_dim = MeasureTextEx(hint, fontSize);
    const Vector2 hint_dim { text_dim.x + 2 * padding, text_dim.y + padding };
    DrawRectangleRoundedAlign({ hitbox.x + 0.5f * hitbox.width, hitbox.y, hint_dim.x, hint_dim.y}, 0.4f, 5, rgba(15, 15, 15, hint_alpha), CENTER, BOTTOM);
    if (hint_alpha * 2 >= HINT_ALPHA)  // if alpha is half of max, draw text 
        DrawTextAlign(hint, hitbox.x + 0.5f * hitbox.width, hitbox.y - hint_dim.y * 0.5f, fontSize, WHITE, CENTER, CENTER);
}

float Button::get_width()
{
    return hitbox.width;
}

float Button::get_height()
{
    return hitbox.height;
}


