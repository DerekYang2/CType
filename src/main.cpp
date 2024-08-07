/********************************************************************************
*   MIT License
*   
*   Copyright (c) 2023 Derek Yang (@DerekYang2)
*   
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*   
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*   
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*   
*********************************************************************************/
#define CTYPE_VERSION "v0.1"

/**
 * TODO:
 * - Words mode
 * - User profile, test history, etc
*/

#include <filesystem>
#include <thread>
#include <mutex>
#include "raylibcustom.h"        
#include <math.h>
#include <chrono>
#include "globals.h"
#include "Theme.h"
#include "ResourceInit.h"
#include "SoundEffects.h"
#include "UIAlloc.h"
#include "Button.h"
#include "ObjectMacros.h"
#include "Stopwatch.h"
#include "ShaderEmbed.h"
#include "WordList.h"
#include "TextGeneration.h"
#include "TextDrawer.h"
#include "IOHandler.h"
#include "StatusHandling.h"
#include "WpmLogger.h"
#include "BezierPath.h"
#include "Graph.h"
#include "Toggle.h"
#include "ToggleGroup.h"
#include "TextPanelV.h"
#include "SettingBar.h"
#include "InputBox.h"
#include "PopupHandler.h"
#include "TestInfo.h"
#include "RectPreview.h"
#include "Settings.h"
#include "UserData.h"
#include "ToggleSpawn.h"
#include "FileExplorer.h"
#include "HorizontalGroup.h"
#include "VerticalGroup.h"
#include "Notification.h"
#include "FontToggle.h"
// Init extern variables ------------------------------------------------------------------
/* Theme theme(
    rgb(232, 233, 236), // background
    rgb(78, 93, 146), // main
    rgb(53, 54, 76), // caret 
    rgb(133, 136, 163), // sub
    rgb(201, 201, 204), // sub-alt
    rgb(53, 54, 76), // text
    rgb(198, 77, 123), // error
    rgb(147, 57, 91) // error_extra
);  */
Theme theme;
// Window Variables
int gameScreenWidth = 1920, gameScreenHeight = 1080;
int windowWidth = gameScreenWidth, windowHeight = gameScreenHeight;
bool mouse_focus = true;

// FONT SIZES
FontMeasure font_measure(23, 27, 35, 50);

// GLOBAL VARS
unordered_set<int> scene_ids[SCENE_COUNT];
int scene = START;
int pending_scene = -1;
Vector2 mouse;
int globalFrame; // Frames printed in the game scene
unordered_map<string, Texture> textureOf;
int empty_i = 0;

string generated_chars = "";
/**
 * STATUS:
 * first = 0 -> store correct char
 * first = 1 -> store incorrect char, typed
 * first = 2 -> store incorrect char, not typed
 * first = 3 -> store incorrect char, extra typed
*/
vector<pair<char, char>> char_status;
vector<Word> words;
int word_i = 0;

// Init mt19937 with random seed
mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
Font font; // Font: UI font
string font_path = "";
float font_spacing;

UIAlloc ui_objects(200);
Shader shader;
Vector2 char_dimension[CHAR_MAX + 1];

// HANDLERS
TextDrawer drawer;
IOHandler io_handler[SCENE_COUNT];
WpmLogger wpm_logger;
TestInfo test_info;
TextGenerator text_gen;

bool drawing_block;
float drawing_x, drawing_y;

// settings UI
TogglePanel* behavior_panel;
unordered_map<string, ToggleGroup*> setting_toggle;
vector<InputBox*> input_boxes[SCENE_COUNT];
ThemeToggle* theme_toggle;

StatusCount status_count;

string default_settings;
// multithreading
unordered_map<string, pair<unsigned char*, int>> loaded_file_data;
std::mutex global_mutex;

// END init extern variables ----------------------------------------------------------------
const string SCREENSHOT_FOLDER = "screenshots", FONTS_FOLDER = "fonts";
const string start_label = "default test", settings_label = "settings", about_label = "about", user_label = "user";
string cursor_path = "arrow_cursor";  // For custom cursor
float cursor_height = 22.f;
int cursor_id = MOUSE_CURSOR_DEFAULT;

// Notification handler
Notification* notification;
// render textures
RenderTexture2D target;

int final_wpm;
int final_raw_wpm;
int final_accuracy;
int max_word_length;
int consistency;
float elapsed;
// Local UI variables
Button* restart_button;
Graph* graph;
TextPanelV* end_stats;
SettingBar* setting_bar;
ToggleGroup* taskbar;
PopupHandler* time_popup;
ToggleSpawn* dictionary_spawn;

// Window buttons
bool close_window = false;
Button* close_button, * minimize_button;
Toggle* fullscreen_toggle;
// test display vars
int display_wpm_frames = 15;  // frames to update wpm display
int display_wpm;
float max_wpm = 0, cur_wpm = 0;

// UI DIMENSIONS 
float wpm_width = 350;  // width of wpm text 
float graph_width = 1300;
float graph_top = 200;
float graph_height = 600 ;
int mouse_frames;
float scale;
// main local flags 
bool pending_popup_draw = false;
bool repeat_test = false;

void init_test()
{

    load_sound_effects(SOUNDS_FOLDER + "/" + sound_toggle->get_selected());
    drawer = TextDrawer(font, font_measure.large());
    reset_IOHandler(TEST);
    // reset variables
    status_count = StatusCount(0, 0, 0, 0);
    elapsed = 0;
    word_i = 0;
    empty_i = 0;
    wpm_logger.reset();
    char_status.clear();
    
    if (repeat_test)  // Only repeat once when flag is set
    {
        repeat_test = false;
        // Reset word variables
        for (auto& word : words)
        {
            word.reset();
        }
    } else  // Regenerate text
    {
        words.clear();
        generated_chars = "";
        text_gen.set_punctuation(setting_bar->is_toggled("punctuation"));
        text_gen.set_numbers(setting_bar->is_toggled("numbers"));
        text_gen.set_list(dictionary_spawn->get_selected());
        text_gen.generate_text((is_tape_mode ? 1 : 3) * ceil(gameScreenWidth / char_dimension['i'].x));
    } 
}

// scene switch functions
void switch_start()
{
    pending_scene = START;
    taskbar->set_selected(start_label);
}

void switch_settings()
{
    pending_scene = SETTINGS;
    taskbar->set_selected(settings_label);
}

void switch_about()
{
    pending_scene = ABOUT;
    taskbar->set_selected(about_label);
}

void switch_user()
{
    pending_scene = USER;
    taskbar->set_selected(user_label);
}

void switch_popup()
{
    pending_scene = POPUP;
    pending_popup_draw = true;
}

void start_test()
{
    test_info.init(stoi(setting_bar->group_selected()));
    data_json["test time"] = test_info.time;
    data_json["custom time"] = stoi(setting_bar->custom_input());
    
    wpm_logger.start();
    scene = TEST;
    display_wpm = 0;
    cur_wpm = 0;
    max_wpm = 0;
    mouse_frames = 0;  // make disappear
}

void end_test()
{
    wpm_logger.end();
    final_wpm = round(wpm_logger.wpm());
    test_info.push_final();
    final_raw_wpm = round(wpm_logger.raw_wpm());
    final_accuracy = round(100 * wpm_logger.accuracy());
    consistency = round(100 * (1 - test_info.variation()));
    scene = END;
        
    test_info.update_graph(graph);

    // character status can get too wide 
    string char_status = TextFormat("%d/%d/%d/%d", status_count.correct, status_count.incorrect, status_count.missing, status_count.extra);

    
    string test_mode_append = "";
    test_mode_append += text_gen.get_punctuation() ? "\npunctuation" : "";
    test_mode_append += text_gen.get_numbers() ? "\nnumbers" : "";
    const float extra_large_size = MeasureFontSize("A", INT_MAX, 80);
    string cropped_list = text_gen.list_name();
    if (cropped_list.size() > 20)
        cropped_list = cropped_list.substr(0, 17) + "...";
    end_stats->init({ {"wpm", font_measure.title()}, {"acc", font_measure.title()}, {"raw", font_measure.medium()}, {"consistency", font_measure.medium()}, {"characters", font_measure.medium()}, {"test type", font_measure.medium()} },
                    // Values
                    {
                        {t_s(final_wpm), extra_large_size},
                        {TextFormat("%d%%", final_accuracy), extra_large_size},
                        {t_s(final_raw_wpm), font_measure.title()},
                        {TextFormat("%d%%", consistency), font_measure.title()},
                        {char_status, min(font_measure.title(), MeasureFontSize(char_status, wpm_width - 65))},
                        {"time " + t_s(test_info.time) + "\n" + cropped_list + test_mode_append, font_measure.medium()}
                    },
                    // Hints 
                    {
                        TextFormat("%.2f wpm", wpm_logger.wpm()),
                        TextFormat("%.2f%% characters correct", 100 * wpm_logger.accuracy()),
                        TextFormat("%.2f wpm", wpm_logger.raw_wpm()),
                        TextFormat("%.2f%% variance", 100 * (1 - test_info.variation())),
                        "correct/incorrect/missing/extra",
                        text_gen.list_name()
                    });
}


void update_mouse()
{
    // Update virtual mouse (clamped mouse value behind game screen)
    Vector2 old_mouse = mouse;
    Vector2 real_mouse = GetMousePosition();
    mouse.x = (real_mouse.x - (screen_width() - (gameScreenWidth * scale)) * 0.5f) / scale;
    mouse.y = (real_mouse.y - (screen_height() - (gameScreenHeight*scale))*0.5f)/scale;
    // DON'T CLAMP: mouse = Vector2Clamp(mouse, (Vector2){ 0, 0 }, (Vector2){ (float)gameScreenWidth, (float)gameScreenHeight });

    if (scene == TEST || scene == END)
    {
        if (mouse == old_mouse)
        {
            mouse_frames--;
            mouse_frames = max(0, mouse_frames);
        }
        if (mouse != old_mouse)
        {
            if (!mouse_focus)  // refocus mouse
            {
                mouse_focus = true;
                ShowCursor();
                //cursor_path = "arrow_cursor";
            }
            mouse_frames = 5 * 60;
        }
        if (mouse_focus && mouse_frames <= 0)  // unfocus mouse
        {
            mouse_focus = false;
            HideCursor();
            //cursor_path = "";
        }
    } else if (scene == START || scene == SETTINGS)
    {
        if (!mouse_focus)
        {
            mouse_focus = true;
            ShowCursor();
            //cursor_path = "arrow_cursor";
        }
    }
}

void update_start()
{
    // Tape mode <-> normal mode changes drawer.y position
    dictionary_spawn->set_pos(gameScreenWidth * 0.5f, drawer.get_top_y() - 2 * font_measure.medium_height);
    restart_button->set_pos(0.5f * (gameScreenWidth - font_measure.large_height), drawer.get_bottom_y() + font_measure.large_height);

    if (IsKeyPressed(KEY_ESCAPE))
    {
        switch_settings();
        return;
    }
    if (IsKeyPressed(KEY_TAB) || setting_bar->needs_update() || (dictionary_spawn->get_selected() != text_gen.list_name()))  // restart test 
    {
        data_json["punctuation"] = (int)setting_bar->is_toggled("punctuation");
        data_json["numbers"] = (int)setting_bar->is_toggled("numbers");
        data_json["dictionary"] = "'" + dictionary_spawn->get_selected() + "'";
        init_test();
        notification->spawn_notification("notice", "test reloaded", 2, &textureOf["reload"]);
        return;
    }
    
    if (IsKeyPressed())  // press any key except for space to start
    {
        if (setting_toggle["strict space"]->get_selected() == "on" || !IsKeyPressed(KEY_SPACE))  // if strict space on, any key start, otherwise must not be space press
        {
            start_test();
            notification->kill();
        }
    }
} 

void update_test()
{
    text_gen.generate_text((is_tape_mode?1:2) * ceil(gameScreenWidth / char_dimension['i'].x));
    wpm_logger.update();
    test_info.update();
    elapsed = wpm_logger.get_elapsed();

    cur_wpm = wpm_logger.wpm();
    max_wpm = max(max_wpm, cur_wpm);
    
    if (globalFrame % display_wpm_frames == 0)  // update wpm display
    {
        display_wpm = round(cur_wpm);
    }
    
    if (elapsed >= test_info.time)
    {
        end_test();
    }

    // Only activate button if cursor is on screen
    if (!IsCursorHidden())
        restart_button->update();

    // Restart test
    if (IsKeyPressed(KEY_TAB))  
    {
        switch_start();
        notification->spawn_notification("notice", "test reloaded", 2, &textureOf["reload"]);
    }

    // Play sound on key press
    if (IsKeyPressed() || IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT))
    {
        play_sound();
    }
}

void update_about() {

}

void update_user() {
    
}

void update_taskbar()
{

    if (taskbar->was_pressed())
    {

        int taskbar_scene = -1;
        if (taskbar->get_selected() == start_label)  // home or test
        {
            taskbar_scene = START;
        } else if (taskbar->get_selected() == settings_label)  // settings
        {
            taskbar_scene = SETTINGS;
        } else if (taskbar->get_selected() == about_label)  // about
        {
            taskbar_scene = ABOUT;
        } else if (taskbar->get_selected() == user_label)  // user
        {
            taskbar_scene = USER;
        }

        if (taskbar_scene != scene)  // taskbar scene changed
        {
            pending_scene = taskbar_scene;
        }
    }
}

void update_end()
{
    if (IsKeyPressed(KEY_TAB))
        switch_start();
}

// Updates on any scene 
void global_update()
{
    close_button->update();
    minimize_button->update();
    fullscreen_toggle->update();
    if ((fullscreen_toggle->toggled() && !IsWindowFullscreen()) || (!fullscreen_toggle->toggled() && IsWindowFullscreen()))  // toggle fullscreen
    {
        ToggleFullscreen();
    }
    // Update cursor
    SetMouseCursor(cursor_id);
}

// TODO: do something for taskbar black background (for black themes)
void draw_taskbar()
{
    Rectangle taskbar_rect = taskbar->bounding_box();
    float padding = taskbar_rect.width * 0.05f;
    taskbar_rect.x -= padding, taskbar_rect.width += 2 * padding;
    taskbar_rect.height *= 2;  // goes of screen
    DrawRectangleRounded(taskbar_rect, 0.15f, 5, theme.sub_alt);

    // Draw selected index
    int index = taskbar->selected_index();
    float icon_w = taskbar->get_height();
    float x = taskbar_rect.x + padding + icon_w * (index + 0.5f);  // taskbar height = width since square
    DrawRectangleRoundedAlign(x, gameScreenHeight, icon_w * 0.4f, icon_w * 0.1f, 1.f, 5, theme.main, CENTER, CENTER);

    // Redraw in front
    taskbar->draw();
    taskbar->draw_hint();
}
void draw_start()
{
    ClearBackground(theme.background);  // Clear render texture background color
    drawer.draw();
    if ((globalFrame/30) & 1)  // if active or Inactive, blink half of the time
    {
        drawer.draw_caret();
    }
    draw_taskbar();
    notification->draw();
}

/* deque<float> stored_wpm;
deque<float> stored_current_wpm; */
Shader test_shader;

void draw_test()
{
    ClearBackground(theme.background);  // Clear render texture background color
    // TEST draw vel
/*     stored_wpm.push_back(wpm_logger.wpm());
    if (stored_wpm.size() > 1000)
        stored_wpm.pop_front();
    stored_current_wpm.push_back(wpm_logger.instant_wpm());
    if (stored_current_wpm.size() > 1000)
        stored_current_wpm.pop_front(); */
    
/*     const float scale_f = 2;
    for (int x_pos = stored_wpm.size() - 1; x_pos >= 0; x_pos--)
        DrawRectangleRec(formatRect(Rectangle(x_pos, 500 - scale_f*stored_wpm[x_pos], 1, scale_f*stored_wpm[x_pos])), RED);
    for (int x_pos = stored_current_wpm.size() - 1; x_pos >= 0; x_pos--)
        DrawRectangleRec(formatRect(Rectangle(x_pos, 500 - scale_f*stored_current_wpm[x_pos], 1, 10)), BLUE); */

    if (setting_toggle["show wpm"]->get_selected() == "on")
    {
        const string time_text = " " + convertSeconds((int)round(test_info.time - elapsed), test_info.time) + " ",
                     wpm_text = TextFormat("% 3d", display_wpm);
        const float time_percent = elapsed / test_info.time, dash_percent = (cur_wpm / (1.1f * max_wpm + 10));
        drawer.draw_time(time_text, wpm_text, time_percent, dash_percent, !is_tape_mode);
    }
    // DRAW TYPING TEST TEXT
    drawer.draw();
    
    if (io_handler[TEST].active_cursor())  // if active or Inactive, blink half of the time
    {
        drawer.draw_caret();
    }
    if (!IsCursorHidden() && elapsed >= 0.5f)
    {
        restart_button->draw();
        restart_button->draw_hint();
    }
    notification->draw();
}

void draw_end()
{
    ClearBackground(theme.background);  // Clear render texture background color
    notification->draw();
}

void draw_about()
{
    ClearBackground(theme.background);
    //DrawTextAlign("About", gameScreenWidth / 2, gameScreenHeight / 2, font_measure.large(), theme.main, CENTER, CENTER);
    draw_taskbar();
}

void draw_user()
{
    ClearBackground(theme.background);
    DrawTextAlign("User (not implemented yet)", gameScreenWidth / 2, gameScreenHeight / 2, font_measure.large(), theme.main, CENTER, CENTER);
    draw_taskbar();
}

void draw_popup()
{
    if (pending_popup_draw)
    {
        DrawRectangle(0, 0, gameScreenWidth, gameScreenHeight, rgba(0, 0, 0, 0.5f));
        pending_popup_draw = false;
    }
}

void global_draw()
{
    close_button->draw();
    minimize_button->draw();
    fullscreen_toggle->draw();
    if (scene != POPUP)
    {
        DrawTextAlign(CTYPE_VERSION, 10, gameScreenHeight - 10, font_measure.small(), theme.sub, LEFT, BOTTOM);  // Draw version
    }
}

void draw_cursor()
{
    if (cursor_path.empty() || !IsCursorOnScreen()) return;
    Texture cursor = textureOf[cursor_path];
    float scale = cursor_height / max(cursor.width, cursor.height);
    bool centered = (cursor_path == "link_cursor");
    float offset = centered ? cursor.width * scale * 0.5f : 0;
    Vector2 abs_mouse = GetMousePosition();
    abs_mouse.x -= offset;
    DrawTextureEx(cursor, abs_mouse, 0, scale, theme.main);
    
    cursor_path = "arrow_cursor";  // set back to default
}

void draw_logo()
{
    DrawTextureEx(textureOf["logo"], {25, 25}, 0, 50.f / textureOf["logo"].height, theme.main);
}
//NOTE: C:/Windows/Fonts/segoeui.ttf - SEGOE UI PATH

void set_rand_font()
{
    vector<string> font_paths = directory_files("C:/Windows/Fonts", ".ttf");
    string font_path = font_paths[rand_int(0, font_paths.size() - 1)];
    init_font(font_path);
}

void set_icon()
{
    Image icon_img = { 0 }; 
    icon_img.format = LOGO_ICON_FORMAT; 
    icon_img.width = LOGO_ICON_WIDTH; 
    icon_img.height = LOGO_ICON_HEIGHT; 
    icon_img.data = LOGO_ICON_DATA; 
    icon_img.mipmaps = 1;
    Image imCopy = ImageCopy(icon_img);
    ImageColorTint(&imCopy, theme.main);
    SetWindowIcon(imCopy);
}

void init()
{
    load_sdf_shader();
    //test_shader = LoadShader(0, "./fonts/test2.frag");
    //load_base_font("fonts/RobotoMono.ttf");
    // init_font("fonts/RobotoMono.ttf");

    //set_rand_font();
    
    init_raw_data;
    for (auto& [key, texture] : textureOf)
    {
        SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
    }
    init_dictionary_names();
    load_user_data();
    init_settings();
    load_sound_effects("sounds/nk_creams");
    set_icon();

    // Notification
    notification = new Notification(10, font_measure.medium());

    // Get selected test time and other user data
    string selected_time = data_json["test time"].as<string>();
    string custom_time = data_json["custom time"].as<string>();
    bool punctuation_on = data_json["punctuation"].as<int>();
    bool numbers_on = data_json["numbers"].as<int>();
    string selected_dictionary = data_json["dictionary"].as<string>();
    // Window UI
    close_button = new Button(gameScreenWidth - 50, 0, 50, &textureOf["exit_icon"], [] {close_window = true;});
    fullscreen_toggle = new Toggle(gameScreenWidth - 100, 0, 50, false, &textureOf["unfullscreen"], &textureOf["fullscreen"]);
    minimize_button = new Button(gameScreenWidth - 150, 0, 50, &textureOf["minimise_icon"], [] {MinimizeWindow();});

    // STARTING UI ---------------------------------------------------------------
    // custom time popup
    Textbox* p_title = new Textbox(0, 0, 450, font_measure.title_height, "Test Duration", 15, "main", false);
    Textbox* p_description = new Textbox(0, 0, 400, 50, "Enter a custom duration in seconds, between 2 and 10000.\nCustom duration is:\n%s", font_measure.medium(), "sub", true);
    Button* p_button = new Button(0, 0, 200, font_measure.title_height, "Ok", nullptr);
    InputBox* input_box = new InputBox(0, 0, 380, font_measure.large_height, custom_time, true, [](string s) -> string {
        // format s as time 
        try
        {
            string formatted = format_time(s);
            if (formatted.size() <= 2) formatted += "s";  // if less than a minute, add seconds suffix
            return formatted;
        }
        catch (...)
        {
            return "default";
        }
    });
    input_box->set_range(2, 10000);
    input_box->set_IOHandler(POPUP);
    
    time_popup = new PopupHandler(gameScreenWidth * 0.5f, gameScreenHeight * 0.5f, 500, 500, p_title, p_description, input_box, p_button);
    ui_objects.alloc(time_popup, POPUP);
    
    const float bar_h = font_measure.medium_height * 1.15f;
    Toggle* punctuation = new Toggle(0, 300, bar_h, punctuation_on, "punctuation", "at_icon");
    Toggle* numbers = new Toggle(0, 300, bar_h, numbers_on, "numbers", "hashtag_icon");
    const vector<string> options = { "5", "15", "30", "60", "120", "custom" };
    ToggleGroup* time_toggles = new ToggleGroup(0, 300, bar_h, 0, options);
    if (find(options.begin(), options.end(), selected_time) == options.end())  // selected  time was not found in default options 
        time_toggles->set_selected("custom");
    else
        time_toggles->set_selected(selected_time);
    
    //new_ToggleGroup(START, 0, 300, 50, 0, { "15", "30", "60", "120" });
    setting_bar = new SettingBar(gameScreenWidth / 2, 300, { punctuation, numbers }, time_toggles, time_popup);
    ui_objects.alloc(setting_bar, START);

    // List of dictionaries
    unordered_set<string> in_list;
    vector<string> english_list = { "english", "english 1k", "english 5k", "english 10k", "english 25k", "english 450k", "english commonly misspelled", "english contractions", "english doubleletter" };
    english_list.erase(std::remove_if(english_list.begin(), english_list.end(), [](string str) { return !word_list.contains(str);}), english_list.end());  // remove if not in word_list
    in_list.insert(english_list.begin(), english_list.end());

    vector<string> code_list;
    for (auto& [key, value] : word_list)
    {
        if (starts_with(key, "code "))
        {
            code_list.push_back(key);
            in_list.insert(key);
        }
    }
    sort(code_list.begin(), code_list.end());
    
    vector<string> other_list;
    for (auto& [key, value] : word_list)
    {
        // If not in a previous list
        if (!in_list.contains(key))
            other_list.push_back(key);
    }
    sort(other_list.begin(), other_list.end());
    vector<string> dictionary_names;
    // Add lists with proper order into dictionary_names
    dictionary_names.insert(dictionary_names.end(), english_list.begin(), english_list.end());
    dictionary_names.insert(dictionary_names.end(), other_list.begin(), other_list.end());
    dictionary_names.insert(dictionary_names.end(), code_list.begin(), code_list.end());

    // Dictionary panel
    p_title = new Textbox(0, 0, 450, font_measure.title_height, "Select Language", 15, "main", false);
    p_description = new Textbox(0, 0, 600, 50, "Dictionary directory: " + absolute_path(DICTIONARY_FOLDER) + "\nCurrently selected: \n%s", font_measure.small(), "sub", true);
    p_button = new Button(0, 0, 200, font_measure.title_height, "Ok", nullptr);
    PopupHandler* dictionary_popup = new PopupHandler(gameScreenWidth * 0.5f, gameScreenHeight * 0.5f, 700, 40, 10, p_title, p_description, p_button, dictionary_names, selected_dictionary);
    ui_objects.alloc(dictionary_popup, POPUP);

    // dictionary panel spawner
    drawer = TextDrawer(font, font_measure.large());  // Create drawer to button position
    dictionary_spawn = new ToggleSpawn(gameScreenWidth * 0.5f, drawer.get_top_y() - 2*font_measure.medium_height, font_measure.medium_height, dictionary_popup, &textureOf["globe"]);
    ui_objects.alloc(dictionary_spawn, {START, POPUP});

    const float taskbar_height = 75;
    taskbar = new ToggleGroup(gameScreenWidth / 2, gameScreenHeight - taskbar_height, taskbar_height, 0, { "keyboard", "settings_icon", "user", "info"}, { start_label, settings_label, user_label, about_label}, true);
    ui_objects.alloc(taskbar, {START, SETTINGS, USER, ABOUT});

    // TEST UI ---------------------------------------------------------------
    restart_button = new Button(0.5f * (gameScreenWidth - font_measure.large_height), drawer.get_bottom_y() + font_measure.large_height, font_measure.large_height, &textureOf["reload"], [] {
        switch_start();
        notification->spawn_notification("notice", "test reloaded", 2, &textureOf["reload"]);
    }, "restart test");
    ui_objects.alloc(restart_button, START);  // Auto handled in start scene, manually handled in test scene

    // ENDING UI ---------------------------------------------------------------
    Button* new_button = new Button(0, 0, font_measure.large_height, &textureOf["next"], [&] {
        switch_start();
    }, "new test");
    Button* repeat_button = new Button(0, 0, font_measure.large_height, &textureOf["reload"], [&] {
        repeat_test = true;
        switch_start();
    }, "repeat test");
    
    graph = new Graph(wpm_width + (gameScreenWidth - (graph_width + wpm_width)) * 0.5f, graph_top, graph_width, graph_height, 4), ui_objects.alloc(graph, END);
    end_stats = new TextPanelV(0.5f * (gameScreenWidth - (wpm_width + graph_width)), graph_top, wpm_width, graph_height + 20), ui_objects.alloc(end_stats, END);

    Button* screenshot_button = new Button(0, 0, font_measure.large_height, &textureOf["image"], [&] {
        if (!notification->is_alive())  // If screenshot notif wears off
        {
            string file_name = SCREENSHOT_FOLDER + "/Screenshot_" + UnixTimeToDateString(unix_time) + ".png";
            replace(file_name.begin(), file_name.end(), ' ', '_');
            replace(file_name.begin(), file_name.end(), ':', '-');
            //TakeScreenshot(file_name.c_str());
            ExportImage(TextureToImage(target.texture, Rectangle(75, graph_top + 25, gameScreenWidth - 2 * 75, graph_height + 90)), file_name.c_str());
            notification->spawn_notification("success", "saved to screenshots folder", 3, &textureOf["image"]);
        }
    }, "save screenshot");

    HorizontalGroup * end_buttons = new HorizontalGroup(gameScreenWidth * 0.5f, 0.5f * (gameScreenHeight + (graph_top + graph_height)), font_measure.large_height, { new_button, repeat_button, screenshot_button}, true);
    ui_objects.alloc(end_buttons, END);
    
    // ABOUT UI ---------------------------------------------------------------
    const float ABOUT_PADDING = 200;
    TextPanelV* about_panel = new TextPanelV(ABOUT_PADDING, ABOUT_PADDING, gameScreenWidth - 2 * ABOUT_PADDING,
    {
        {"About", font_measure.title()}, {"Word Set", font_measure.large()}, {"Keybinds", font_measure.large()}, {"Customization", font_measure.large()},
    },
    {
        {"CType: Minimalistic Typing Test in C/C++.", font_measure.medium()},
        {"By default, 'english' dictionary contains the 200 most common words in the English language. Dictionary files are located in: " + absolute_path(DICTIONARY_FOLDER) + ". These dictionaries are the same as those in Monkeytype.", font_measure.medium()},
        {"tab - restart test\nescape - toggle between test and settings", font_measure.medium()},
        {"Buttons above link to folders containing fonts, languages, themes, etc. Custom files may be directly added and loaded by application, as long as valid. You may look at already existing files to see what is valid. \nOnly .ttf and .otf fonts are supported. \nNote that css themes files are not actually parsed, just read for certain tokens. \nDictionary files only support standard ASCII for now.", font_measure.medium()}
    }, false);
    Button* github_button = new Button(0, 0, font_measure.large_height, "github", [] {
        OpenURL("https://github.com/DerekYang2/CType");
    }, "https://github.com/DerekYang2/CType");
    Button * reveal_explorer = new Button(0, 0, font_measure.large_height, "file explorer", [] {
        open_path(current_dir()); 
    }, "application location");
    Button * fonts_reveal = new Button(0, 0, font_measure.large_height, "fonts", [] {
        open_path(FONTS_FOLDER);
    }, "fonts folder (.ttf)");
    Button* languages_reveal = new Button(0, 0, font_measure.large_height, "languages", [] {
        open_path(DICTIONARY_FOLDER);
    }, "languages folder (.json)");
    Button* themes_reveal = new Button(0, 0, font_measure.large_height, "themes", [] {
        open_path(THEMES_FOLDER);
    }, "themes folder (.css)");
    Button* screenshot_reveal = new Button(0, 0, font_measure.large_height, "screenshots", [] {
        open_path(SCREENSHOT_FOLDER);
    }, "saved screenshots folder");
    Button* sounds_reveal = new Button(0, 0, font_measure.large_height, "sounds", [] {
        open_path(SOUNDS_FOLDER);
    }, "keyboard sounds folder (.wav)");
    
    HorizontalGroup* link_buttons = new HorizontalGroup(0, 0, font_measure.large_height, { github_button, reveal_explorer, fonts_reveal, languages_reveal, themes_reveal, screenshot_reveal, sounds_reveal}, true);

    VerticalGroup* about_group = new VerticalGroup(gameScreenWidth * 0.5f, ABOUT_PADDING * 0.5f, font_measure.large_height, { link_buttons, about_panel }, false);
    ui_objects.alloc(about_group, ABOUT);

    reset_IOHandler(POPUP);
}


std::function<void()> init_loading_thread()
{
    namespace fs = std::filesystem;
    // Get font file data
    vector<string> font_paths = directory_files(FONTS_FOLDER, ".ttf");
    for (string path : font_paths)
    {
        loaded_file_data.insert({path, {nullptr, 0}});
    }
    // Get sound file data 
    for (const auto& path : fs::directory_iterator(SOUNDS_FOLDER))
    {
        if (fs::is_directory(path))
        {
            vector<string> wav_paths = directory_files(path.path().string(), ".wav");
            for (string file_path : wav_paths)
            {
                replace(file_path.begin(), file_path.end(), '\\', '/');  // Be consistent with backslash direction
                loaded_file_data.insert({ file_path, {nullptr, 0} });
            }
        }
    }
    
    // Loading function
    auto test_func = [&] {
        std::lock_guard<std::mutex> lock(global_mutex);
        Stopwatch sw;
        sw.start();
        for (auto& [path, value] : loaded_file_data)
        {
            // Loading file to memory
            unsigned int fileSize = 0;
            unsigned char* fileData = LoadFileData(path.c_str(), &fileSize);
            loaded_file_data[path] = make_pair(fileData, fileSize);
        }
        cout << "Loading time: " << sw.ms() << endl;
        
     };
    return test_func;
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    thread loading_thread(init_loading_thread());
    /**
     * TECHNICAL INITIALIZATION
    */
    // Enable config flags for resizable window and vertical synchro
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_WINDOW_UNDECORATED);

    InitWindow(windowWidth, windowHeight, "CType");
    MaximizeWindow();
    SetWindowMinSize(320, 240);
    InitAudioDevice();              
    // Render texture initialization, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);  // Texture scale filter to use
    SetTextureWrap(target.texture, TEXTURE_WRAP_CLAMP);  // For whatever reason, by default render texture will leak one pixel wrap around
        
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    SetExitKey(KEY_NULL);

    if (loading_thread.joinable())  // Join loading thread before initializing
        loading_thread.join();
    init();
    init_test();
    //open_path(absolute_path("fonts"));

    // HideCursor();  // only hide for custom cursor
    // example shader init
/*     int textLoc = GetShaderLocation(test_shader, "texture0");
    int vecLoc = GetShaderLocation(test_shader, "resolution");
    SetShaderValueTexture(test_shader, textLoc, target.texture);
    static const float arr[2] = { gameScreenWidth , gameScreenHeight };
    SetShaderValue(test_shader, vecLoc, arr, SHADER_UNIFORM_VEC2);
    bool shader_on = false; */
    // timer funcs
    float frame_time = 1;
    Stopwatch frame_timer, application_timer;
    application_timer.start();
    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose() && !close_window)        // Detect window close button 
    {
        frame_timer.start();
        //----------------------------------------------------------------------------------
        // Compute required framebuffer scaling
        scale = min((float)screen_width() / gameScreenWidth, (float)screen_height() / gameScreenHeight);
        update_mouse();
        set_cursor(MOUSE_CURSOR_DEFAULT);
        // Update pending scene
        if (pending_scene != -1)
        {
            scene = pending_scene;
            if (scene == START) 
                init_test();
        }
        pending_scene = -1;
        // Update alloc 
        ui_objects.update();
        // Update
        globalFrame++;
        update_rect_preview();
        // Update UI objects
        for (const int id : scene_ids[scene])
        {
            ui_objects[id]->update();
        }
        update_taskbar();
        
        // must be if instead of of else if so start transitions into test directly
        if (scene == START)
        {
            update_start();  // must be called before test so first char press carries into test
        } else if (scene == SETTINGS)
        {
            update_settings();
        }
        
        io_handler[scene].update();  // update the IO handler of the current scene
        if (scene == TEST)
        {
            update_test();
        }
        if (scene == END)
        {
            update_end();
        }
        if (scene == ABOUT) 
        {
            update_about();
        }
        if (scene == USER)
        {
            update_user();
        }
        if (scene == POPUP)
        {
            
        }

        global_update();
        //shader_on = IsKeyDown(KEY_ENTER);
        //----------------------------------------------------------------------------------
        // Draw everything in the render texture, note this will not be rendered on screen, yet
        BeginTextureMode(target);
        BeginBlendMode(BLEND_CUSTOM_SEPARATE);

        BeginShaderMode(shader);    // Activate SDF font shader    

        if (scene == START)
        {
            draw_start();
        } else if (scene == TEST)
        {
            draw_test();
        } else if (scene == END)
        {
            draw_end();
        } else if (scene == SETTINGS)
        {
            draw_settings();
        } else if (scene == POPUP)
        {
            draw_popup();
        } else if (scene == ABOUT)
        {
            draw_about();
        } else if (scene == USER)
        {
            draw_user();
        }
        
        if (setting_toggle["debug mode"]->get_selected() == "on")
        {
            int max_fps = 1000.0 / frame_time;
            DrawText(TextFormat("MAX FPS: %d | ELAPSED TIME: %.2f", max_fps, frame_time), 0, gameScreenHeight - 30, 25, BLACK);
        }

        for (const int id : scene_ids[scene])
        {
            ui_objects[id]->draw();
        }
        for (const int id : scene_ids[scene])
        {
            ui_objects[id]->draw_hint();
        }
        draw_rect_preview();
        if (scene == SETTINGS)
        {
            draw_borders();
        }
        global_draw();
        EndShaderMode();
        draw_logo();
        EndBlendMode();
        EndTextureMode();
        // Draw render texture onto real screen ---------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);     // Clear screen background
        //if (shader_on) BeginShaderMode(test_shader);
        // Draw render texture to screen, properly scaled
        DrawTexturePro(target.texture, (Rectangle) { 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                           (Rectangle){ (screen_width() - ((float)gameScreenWidth*scale))*0.5f, (screen_height() - ((float)gameScreenHeight*scale))*0.5f,
                           (float)gameScreenWidth* scale, (float)gameScreenHeight* scale
        }, (Vector2) { 0, 0 }, 0.0f, WHITE);
        //draw_cursor(); // custom cursors
        //if (shader_on) EndShaderMode();
        if (globalFrame % 30 == 0)
            frame_time = frame_timer.ms();
        EndDrawing();
        //--------------------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    write_settings();                   // update settings file
    write_user_data();
    UnloadRenderTexture(target);        // Unload render texture
    for (auto& [path, texture] : textureOf)
        UnloadTexture(texture);
    UnloadShader(shader);
    CloseWindow();                      // Close window and OpenGL context
    CloseAudioDevice();
    //--------------------------------------------------------------------------------------
    refresh_themes();
    return 0;
}
