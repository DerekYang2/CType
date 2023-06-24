#include "RectPreview.h"

void update_rect_preview()
{
    if (setting_toggle["debug mode"]->get_selected() == "off")
    {
        drawing_block = false;
        return;
    }
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (!drawing_block)
        {
            drawing_block = true;
            drawing_x = mouse.x;
            drawing_y = mouse.y;
        }
    }

    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        drawing_block = false;
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        Rectangle r = formatRect(Rectangle(drawing_x, drawing_y, mouse.x - drawing_x, mouse.y - drawing_y));
        cout << TextFormat("Rectangle(%.0f, %.0f, %.0f, %.0f)", r.x, r.y, r.width, r.height) << endl;
    }
}

void draw_rect_preview()
{
    if (drawing_block)
    {
        Rectangle r = formatRect(Rectangle(drawing_x, drawing_y, mouse.x - drawing_x, mouse.y - drawing_y));
        DrawTextAlign(TextFormat("(%.0f, %.0f, %.0f, %.0f)", r.x, r.y, r.width, r.height), r.x, r.y, 16, BLACK, LEFT, BOTTOM);
        DrawRectangleLinesEx(r, 2, RED);
    }
}