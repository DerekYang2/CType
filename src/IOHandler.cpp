#include "IOHandler.h"

/**
 * TODO: ctrl + hold key doesn't work, GetCharPressed() doesn't return valid char when ctrl + char is held
*/

IOHandler::IOHandler()
{
    back_frames = 0;
    active_frames = 0;
    add_function = NULL, back_function = NULL;
}

IOHandler::IOHandler(function<void(char)> add_function, function<void()> back_function) : add_function(add_function), back_function(back_function)
{
    back_frames = 0;
    active_frames = 0;
}

/**
 * NOTE: strange behavior with autohotkey correct, pressing space right too quickly after wrong word doesn't work
 * Example: "porblem" should become "problem ", but results in "proble m"
 * for some reason, space comes before 'm'
 * add function: update_status(char_pressed);
 * back function: update_backspace(); 
*/
void IOHandler::update()
{
    if (add_function == NULL) return;
    int key_pressed = GetKeyPressed();
    
    fill(handled_press, handled_press + CHAR_MAX + 1, false);
    while (key_pressed)  // IF PRESSED
    {
        active_frames = inactive_time;
        char char_pressed = convertKey(key_pressed);
        // shift the character pressed
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
            char_pressed = shiftChar(char_pressed);
        if (char_pressed != 0)  // valid character
        {
            add_function(char_pressed);
            handled_press[char_pressed] = true;  // Do not handle it again
        } else if (key_pressed == KEY_BACKSPACE)
        {
            back_function();
        } 
        key_pressed = GetKeyPressed();
    }
    
    // HANDLE held char
    char char_pressed = GetCharPressed();
    while (char_pressed)
    {
        active_frames = inactive_time;
        if (!handled_press[char_pressed])
        {
            add_function(char_pressed);
        }
        char_pressed = GetCharPressed();
    } 

    // Handle held backspace
    if (IsKeyDown(KEY_BACKSPACE))
        back_frames++;
    else
        back_frames = 0;
    
    if (!IsKeyPressed(KEY_BACKSPACE) && back_frames > 30)  // IF HELD
    {
        active_frames = inactive_time;
        back_function();
    }
    
    active_frames--;
    if (active_frames < -2 * blink_time) active_frames = -1;  // prevent overflow
}

bool IOHandler::active_cursor()
{
    return active_frames > 0 || ((-active_frames/blink_time) & 1);
}

void reset_IOHandler(int sceneId)
{
    if (sceneId == TEST)
    {
        io_handler[sceneId] = IOHandler(
        [](char char_pressed) {
            update_status(char_pressed);
        },
        [] {
            update_backspace();
        });
    } else if (sceneId == POPUP)
    {
        io_handler[sceneId] = IOHandler(
        [](char char_pressed) {
            for (InputBox* input_box : input_boxes[POPUP])
            {
                input_box->push_char(char_pressed);
            }
        },
        [] {
            for (InputBox* input_box : input_boxes[POPUP])
            {
                input_box->pop_char();
            }
        });
    }
}