#include "PopupHandler.h"

PopupHandler::PopupHandler(float x, float y, float width, float height, Textbox* title, Textbox* description, InputBox* input_box, Button* button) : title(title), description(description), input_box(input_box), button(button)
{
    active = return_call = false;
    rect = Rectangle(x, y, width, height);
    // center rect by default 
    rect.x -= rect.width * 0.5f, rect.y -= rect.height * 0.5f;
    
    float c_x = rect.x + rect.width * 0.5f;
    int non_null = 0;
    float height_sum = 0;
    if (title != nullptr) height_sum += title->get_height(), non_null++;
    if (input_box != nullptr) height_sum += input_box->get_height(), non_null++;
    if (description != nullptr) height_sum += description->get_height(), non_null++;
    if (button != nullptr) height_sum += button->get_height(), non_null++;
    float y_gap = (rect.height - height_sum) / (non_null + 1);
    float y_pos = rect.y + y_gap;

    if (title != nullptr)
        title->set_pos(c_x - title->get_width() * 0.5f, y_pos), y_pos += y_gap + title->get_height();
    if (input_box != nullptr)
        input_box->set_pos(c_x - input_box->get_width() * 0.5f, y_pos), y_pos += y_gap + input_box->get_height();
    if (description != nullptr)
        description->set_pos(c_x - description->get_width() * 0.5f, y_pos), y_pos += y_gap + description->get_height();
    if (button != nullptr)
    {
        button->attach_trigger([&] {
            return_call = true;
        });
        button->set_pos(c_x - button->get_width() * 0.5f, y_pos), y_pos += y_gap + button->get_height();
    }
}

PopupHandler::PopupHandler(float x, float y, float width, float toggle_height, int rows, Textbox* title, Textbox* description, Button* button, vector<string> options, string init_option): title(title), description(description), input_box(nullptr), button(button)
{
    active = return_call = false;

    panel = new VerticalToggle(x, y, width, toggle_height, rows, options, init_option);

    float padding = toggle_height;

    float height_sum = padding;
    if (title != nullptr) height_sum += title->get_height() + padding;
    if (description != nullptr) height_sum += description->get_height() + padding;
    if (panel != nullptr) height_sum += panel->get_height() + padding;
    if (button != nullptr) height_sum += button->get_height() + padding;
    
    rect = Rectangle(x, y, width, height_sum);
    // Center rect by default
    rect.x -= rect.width * 0.5f, rect.y -= rect.height * 0.5f;

    // Set all proper positions
    float cx = rect.x + rect.width * 0.5f, y_pos = rect.y + padding;
    if (title != nullptr)
        title->set_pos(cx - title->get_width() * 0.5f, y_pos), y_pos += title->get_height() + padding;
    if (description != nullptr)
        description->set_pos(cx - description->get_width() * 0.5f, y_pos), y_pos += description->get_height() + padding;
    if (panel != nullptr)
        panel->set_pos(cx - panel->get_width() * 0.5f, y_pos), y_pos += panel->get_height() + padding;
    if (button != nullptr)
    {
        button->attach_trigger([&] {
            return_call = true;
        });
        button->set_pos(cx - button->get_width() * 0.5f, y_pos), y_pos += button->get_height() + padding;
    }
}

void PopupHandler::update()
{
    if (active)
    {
        if (input_box != nullptr)
            input_box->update();
        if (panel != nullptr)
            panel->update();
        if (button != nullptr)
            button->update();
        if (description != nullptr)
        {
            if (input_box != nullptr)
                description->set_var_str({ input_box->get_text() });
            if (panel != nullptr)
                description->set_var_str({ panel->get_selected() });
        }
        if ((!CheckCollisionPointRec(mouse, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) || IsKeyPressed(KEY_ENTER))
        {
            return_call = true;
        }
    }
    if (return_call)
    {
        if (button != nullptr)
            button->reset();
        if (input_box != nullptr)
            input_box->unfocus();
        active = return_call = false;
        scene = init_id;
    }
}

void PopupHandler::draw()
{
    if (active)
    {
        DrawRectangleRounded(rect, 0.1f, 10, theme.background);
        if (title != nullptr)
            title->draw();
        if (input_box != nullptr)
            input_box->draw();
        if (panel != nullptr)
            panel->draw();
        if (description != nullptr)
            description->draw();
        if (button != nullptr)
            button->draw();
    }
}

void PopupHandler::set_active()
{
    active = true;
    init_id = scene;
    if (input_box != nullptr)
        input_box->focus();
}

string PopupHandler::input_text()
{
    if (input_box != nullptr)
        return input_box->get_text();
    return "";
}

string PopupHandler::get_selected()
{
    if (panel != nullptr)
        return panel->get_selected();
    return "";
}

string PopupHandler::input_number()
{
    return input_box->get_number();
}

bool PopupHandler::is_numeric()
{
    return input_box->is_numeric();
}

bool PopupHandler::needs_update()
{
    if (panel != nullptr)
        return panel->was_pressed();
    return false;
}
