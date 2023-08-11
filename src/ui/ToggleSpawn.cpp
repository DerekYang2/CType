#include "ToggleSpawn.h"

ToggleSpawn::ToggleSpawn(float c_x, float y, float height, PopupHandler* popup, Texture* texture) : hitbox{ 0, y, 0, height }, popup(popup), texture(texture)
{
    cx = c_x;  // Center x
    button = new Button(0, 0, hitbox.height, popup->get_selected(), texture, [=] {
        popup->set_active();
        switch_popup();
    });
    hitbox.width = button->get_width();
    hitbox.x = cx - hitbox.width * 0.5f;
    // Set button position
    button->set_pos(hitbox.x, hitbox.y);
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
        hitbox.width = button->get_width();
        hitbox.x = cx - hitbox.width * 0.5f;
        button->set_pos(hitbox.x, hitbox.y);
    }
}

void ToggleSpawn::draw()
{
    button->draw();
}

void ToggleSpawn::set_pos(float x2, float y2)
{
    cx = x2;
    hitbox.x = cx - hitbox.width * 0.5f;
    hitbox.y = y2;
    // Set button position
    button->set_pos(hitbox.x, hitbox.y);
}

bool ToggleSpawn::needs_update()
{
    return popup->needs_update();
}

string ToggleSpawn::get_selected()
{
    return popup->get_selected();
}