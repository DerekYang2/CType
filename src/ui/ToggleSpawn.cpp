#include "ToggleSpawn.h"

ToggleSpawn::ToggleSpawn(float c_x, float y, float height, PopupHandler* popup, Texture* texture) : hitbox{ 0, y, 0, height }, popup(popup), texture(texture)
{
    cx = c_x;  // Center x
    button = new Button(0, 0, 50, popup->get_selected(), [=] {
        popup->set_active();
        switch_popup();
    });
    img_scale = hitbox.height / texture->height;
    space_width = hitbox.height / 2;
    hitbox.width = button->get_width() + space_width + texture->width * img_scale;
    hitbox.x = cx - hitbox.width * 0.5f;
    // Set button position
    button->set_pos(hitbox.x + texture->width * img_scale + space_width, hitbox.y);
}

void ToggleSpawn::update()
{
    // Update trigger button if not already on POPUP
    if (scene != POPUP)
        button->update();
    // Update button text
    if (popup->needs_update())
    {
        button->set_text(popup->get_selected());
        hitbox.width = button->get_width() + space_width + texture->width * img_scale;
        hitbox.x = cx - hitbox.width * 0.5f;
        button->set_pos(hitbox.x + texture->width * img_scale + space_width, hitbox.y);
    }
}

void ToggleSpawn::draw()
{
    Vector2 corner;
    corner.x = hitbox.x;
    corner.y = hitbox.y + 0.5f * (hitbox.height - texture->height * img_scale);
    DrawTextureEx(*texture, corner, 0, img_scale, theme.sub);
    button->draw();
}

bool ToggleSpawn::needs_update()
{
    return popup->needs_update();
}

string ToggleSpawn::get_selected()
{
    return popup->get_selected();
}