#include "Notification.h"
#include "globals.h"

/**
 * background: sub alt
 * small text (sub color) : "success / notice"
 * message : text color
 * outline and texture: sub color
*/

Notification::Notification(float margin, float font_size) : margin(margin), font_size(font_size)
{
    space_width = MeasureTextEx("o", font_size).x;
    lifetime = -1e10;
}

void Notification::spawn_notification(string title, string text, float notification_time, Texture* texture)
{
    stopwatch.start();
    this->title = title;
    this->text = text;
    lifetime = notification_time;
    this->texture = texture;
    // Init dimensioms
    hitbox.height = MeasureTextEx(text, font_size).y + 5 * space_width;
    hitbox.y = gameScreenHeight - hitbox.height - margin;
    text_height = MeasureTextEx(text, font_size).y;
    if (texture != nullptr)
    {
        img_scale = (hitbox.height - 2 * space_width) / texture->height;
        hitbox.width = space_width + MeasureTextEx(text, font_size).x + space_width + texture->width * img_scale + space_width;
    } else
    {
        hitbox.width = space_width + MeasureTextEx(text, font_size).x + space_width;
    }
    hitbox.x = gameScreenWidth - hitbox.width - margin;
}

void Notification::draw()
{
    if (!is_alive())
        return;
    
    const float fade_time = lifetime * 0.25f;
    // Fade out
    float alpha = clamp((lifetime - stopwatch.s()) / fade_time, 0.0, 1.0);
    // Fade in
    const float fade_in = lifetime * 0.05f;
    if (stopwatch.s() <= fade_in)
        alpha = clamp(stopwatch.s() / fade_in, 0.0, 1.0);
    
        
    Color text_col = ColorAlpha(theme.sub, alpha);
    Color main_col = ColorAlpha(theme.main, alpha);
    DrawRectangleRoundedLinesAlign(hitbox, 0.3f, roundedSegments(hitbox.width), 3, text_col);
    DrawRectangleRoundedAlign(hitbox, 0.3f, roundedSegments(hitbox.height), theme.sub_alt);
    if (texture != nullptr)
    {
        Vector2 corner = { hitbox.x + space_width, hitbox.y + 0.5f * (hitbox.height - texture->height * img_scale) };
        DrawTextureEx(*texture, corner, 0, img_scale, text_col);
        DrawTextAlign(title, corner.x + texture->width * img_scale + space_width, hitbox.y + 0.5f * (hitbox.height - text_height), font_measure.small() * 0.9f, main_col, LEFT, BOTTOM);
        DrawTextAlign(text, corner.x + texture->width * img_scale + space_width, hitbox.y + (hitbox.height) * 0.5f, font_size, text_col, LEFT, CENTER);
    } else
    {
        DrawTextAlign(text, hitbox.x + space_width, hitbox.y + (hitbox.height) * 0.5f, font_size, text_col, LEFT, CENTER);
    }
}

void Notification::kill()
{
    lifetime = -1e10;
}

bool Notification::is_alive()
{
    return stopwatch.s() <= lifetime;
}
