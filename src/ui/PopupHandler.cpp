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

void PopupHandler::update()
{
    if (active)
    {
        input_box->update();
        button->update();
        description->set_var_str({input_box->get_text()});
        
        if (!CheckCollisionPointRec(mouse, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            return_call = true;
        }
    }
    if (return_call)
    {
        input_box->unfocus();
        active = return_call = false;
        scene = init_id;
    }
}

void PopupHandler::draw()
{
    DrawRectangleRounded(rect, 0.2f, 10, theme.background);
    title->draw();
    input_box->draw();
    description->draw();
    button->draw();
}

void PopupHandler::set_active()
{
    active = true;
    init_id = scene;
}

string PopupHandler::input_text()
{
    return input_box->get_text();
}