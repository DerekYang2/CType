#include "IOHandler.h"
#include "StatusHandling.h"
#include "WpmLogger.h"

IOHandler::IOHandler()
{
    back_frames = offset_x = offset_vel = vel_target = 0;
    inactive_frames = inactive_time * 60;
}

/**
 * NOTE: strange behavior with autohotkey correct, pressing space right too quickly after wrong word doesn't work
 * Example: "porblem" should become "problem ", but results in "proble m"
 * for some reason, space comes before 'm'
*/
void IOHandler::update()
{
    bool active = false;
    int key_pressed = GetKeyPressed();
    fill(handled_press, handled_press + CHAR_MAX + 1, false);
    
    while (key_pressed)  // IF PRESSED
    {
        active = true;
        char char_pressed = convertKey(key_pressed);
        // shift the character pressed
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
            char_pressed = shiftChar(char_pressed);
        if (char_pressed != 0)  // valid character
        {
            update_status(char_pressed);
            handled_press[char_pressed] = true;  // Do not handle it again
        } else if (key_pressed == KEY_BACKSPACE)
        {
            update_backspace();
        } 
        key_pressed = GetKeyPressed();
    }

    // HANDLE held char
    char char_pressed = GetCharPressed();
    while (char_pressed)
    {
        active = true;
        if (!handled_press[char_pressed])
        {
            update_status(char_pressed);
        }
        char_pressed = GetCharPressed();
    } 

    // Handle held backspace
    if (IsKeyDown(KEY_BACKSPACE))
        back_frames++;
    else
        back_frames = 0;
    
    if (!IsKeyPressed(KEY_BACKSPACE) && (back_frames > 30 && empty_i > 0))  // IF HELD
    {
        update_backspace();
    }
     
    if (active)
        inactive_frames = 0;
    else
        inactive_frames++;

    // update offset 
    float wpm = 120;
    float secperchar = 1.0 / (wpm * 5 / 60);  // predicted seconds to type a char
    vel_target = offset_x / max(1.f, (secperchar * 60));  // cannot divide by anything smaller than 1
    offset_vel += (vel_target - offset_vel) * 0.3f;
    offset_x -= offset_vel;
    drawer.set_offset(offset_x);
}
