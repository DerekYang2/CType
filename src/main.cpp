/*******************************************************************************************
*
*   raylib [core] example - window scale letterbox (and virtual mouse)
*
*   Example originally created with raylib 2.5, last _time updated with raylib 4.0
*
*   Example contributed by Anata (@anatagawa) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2022 Anata (@anatagawa) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include <iostream>
#include "raylibcustom.h"        
#include <math.h>
#include <chrono>
#include "nfd_wrapper.h"
#include "globals.h"
#include "Theme.h"
#include "ResourceInit.h"
#include "RobotoMono.h"
#include "UIAlloc.h"
#include "Button.h"
#include "ObjectMacros.h"
#include "StopWatch.h"
#include "ShaderEmbed.h"
#include "TextGeneration.h"
#include "TextDrawer.h"
#include "IOHandler.h"
#include "WpmLogger.h"
#include "BezierPath.h"
#include "Graph.h"
#include "Toggle.h"
#include "ToggleGroup.h"
#include "TextPanelV.h"
#include "SettingBar.h"
#include "InputBox.h"
#include "TestInfo.h"
#include "RectPreview.h"
#include "Settings.h"
#include "UserData.h"
// Init extern variables ------------------------------------------------------------------
/* Theme theme(
    rgb(232, 233, 236), // background
    rgb(78, 93, 146), // main
    rgb(53, 54, 76), // caret 
    rgb(133, 136, 163), // sub
    rgb(53, 54, 76), // text 
    rgb(198, 77, 123), // error
    rgb(147, 57, 91) // error_extra
);  */
Theme theme{
    rgb(36, 41, 51), // background
    rgb(217, 222, 233), // main
    rgb(217, 222, 233), // caret
    rgb(118, 139, 165), // sub 
    rgb(217, 222, 233), // text 
    rgb(179, 93, 105), // error
    rgb(117, 61, 69) // error_extra
};
// Window Variables
int gameScreenWidth = 1920, gameScreenHeight = 1080;
int windowWidth = gameScreenWidth, windowHeight = gameScreenHeight;
bool mouse_focus = true;

// GLOBAL VARS
unordered_set<int> scene_ids[SCENE_COUNT];
int scene = START;
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
float font_spacing;

UIAlloc ui_objects(100);
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

StatusCount status_count;

string default_settings;

// END init extern variables ----------------------------------------------------------------
string cursor_path = "arrow_cursor";
float cursor_height = 22.f;
// render textures
RenderTexture2D target;

int final_wpm;
int final_raw_wpm;
int final_accuracy;
int max_word_length;
int consistency;
float elapsed;
Graph* graph;
TextPanelV* end_stats;
SettingBar* setting_bar;
ToggleGroup* taskbar;

// test display vars
int display_wpm_frames = 15;  // frames to update wpm display
int display_wpm;
float max_wpm = 0, cur_wpm = 0;
float restart_alpha = 0;
// UI DIMENSIONS 
float wpm_width = 350;  // width of wpm text 
float graph_width = 1300;
float graph_top = 200;
float graph_height = 600 ;
int mouse_frames;
float scale;
bool pending_popup_draw = false;

void init_test()
{
    drawer = TextDrawer(font, 40);
    reset_IOHandler(TEST);
    char_status.clear();
    words.clear();
    // reset variables
    status_count = StatusCount(0, 0, 0, 0);
    elapsed = 0;
    word_i = 0;
    empty_i = 0;
    wpm_logger.reset();
    // generate first chars
    generated_chars = "";
    text_gen.set_list("english");
    if (is_tape_mode) 
        text_gen.generate_text(ceil((gameScreenWidth - (drawer.center + drawer.offset)) / char_dimension['i'].x));
    else
        text_gen.generate_text(3 * ceil(gameScreenWidth / char_dimension['i'].x));
    
}

void start_test()
{
    test_info.init(stoi(setting_bar->group_selected()));
    set_data("test time", test_info.time);

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

    end_stats->init({ {"wpm", 50}, {"acc", 50}, {"raw", 25}, {"consistency", 25}, {"characters", 25}, {"test type", 25}},
                    {
                        {t_s(final_wpm), 100},
                        {TextFormat("%d%%", final_accuracy), 100},
                        {t_s(final_raw_wpm), 50},
                        {TextFormat("%d%%", consistency), 50},
                        {char_status, min(50.f, MeasureFontSize(char_status, wpm_width - 65))},
                        {"time " + t_s(test_info.time) + "\n" + text_gen.list, 25}
                    });
}

void switch_start()
{
    scene = START;
    init_test();
}

void switch_settings()
{
    scene = SETTINGS;
}

void switch_popup()
{
    scene = POPUP;
    pending_popup_draw = true;
}

void update_mouse()
{
    // Update virtual mouse (clamped mouse value behind game screen)
    Vector2 old_mouse = mouse;
    Vector2 real_mouse = GetMousePosition();
    mouse.x = (real_mouse.x - (GetScreenWidth() - (gameScreenWidth * scale)) * 0.5f) / scale;
    mouse.y = (real_mouse.y - (GetScreenHeight() - (gameScreenHeight*scale))*0.5f)/scale;
    mouse = Vector2Clamp(mouse, (Vector2){ 0, 0 }, (Vector2){ (float)gameScreenWidth, (float)gameScreenHeight });

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
    if (restart_alpha > 0)
    {
        restart_alpha -= 1.f / 60;
        restart_alpha = max(restart_alpha, 0.f);
    }
    if (IsKeyPressed(KEY_ESCAPE))
    {
        switch_settings();
        return;
    }
    if (IsKeyPressed(KEY_TAB))  // restart test 
    {
        init_test();
        restart_alpha = 1;
        return;
    }
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_P))
    {
        switch_popup();
        return;
    }
    if (IsKeyPressed())  // press any key except for space to start
    {
        if (setting_toggle["strict space"]->get_selected() == "on" || !IsKeyPressed(KEY_SPACE))  // if strict space on, any key start, otherwise must not be space press
        {
            start_test();
            restart_alpha = 0;
        }
    }
} 

void update_test()
{
    if (is_tape_mode)
    {
        text_gen.generate_text(ceil((gameScreenWidth - (drawer.center + drawer.offset)) / char_dimension['i'].x));
    } else
    {
        text_gen.generate_text(2 * ceil(gameScreenWidth / char_dimension['i'].x));
    }
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
    if (IsKeyPressed(KEY_TAB))  // restart test
    {
        switch_start();
        restart_alpha = 1;
    }
}

void update_taskbar()
{
    if (taskbar->was_pressed())
    {
        int taskbar_scene = -1;
        if (taskbar->get_selected() == "default test")  // home or test
        {
            taskbar_scene = START;
        } else if (taskbar->get_selected() == "settings")  // settings
        {
            taskbar_scene = SETTINGS;
        }
        if (taskbar_scene != scene)  // taskbar scene changed
        {
            if (taskbar_scene == START)
                switch_start();
            else if (taskbar_scene == SETTINGS)
                switch_settings();
        }
    }
}

void update_end()
{
    
}

void draw_taskbar()
{
    Rectangle taskbar_rect = taskbar->bounding_box();
    float padding = taskbar_rect.width * 0.05f;
    taskbar_rect.x -= padding, taskbar_rect.width += 2 * padding;
    taskbar_rect.height *= 2;  // goes of screen
    DrawRectangleRounded(taskbar_rect, 0.15f, 10, theme.background_shade);

    // draw selected index
    int index = taskbar->selected_index();
    float icon_w = taskbar->height();
    float x = taskbar_rect.x + padding + icon_w * (index + 0.5f);  // taskbar height = width since square
    DrawRectangleRoundedAlign(x, gameScreenHeight, icon_w * 0.4f, icon_w * 0.1f, 1.f, 5, theme.main, CENTER, CENTER);
}

void draw_start()
{
    ClearBackground(theme.background);  // Clear render texture background color
    BeginShaderMode(shader);
    drawer.draw();
    if ((globalFrame/30) & 1)  // if active or Inactive, blink half of the time
    {
        drawer.draw_caret();
    }
    Color text_color = theme.main;
    text_color.a = restart_alpha * 255;
    DrawTextAlign("Restarted", gameScreenWidth/2, 450, 30, text_color, CENTER);
    EndShaderMode();
    draw_taskbar();
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
    BeginShaderMode(shader);

    if (setting_toggle["show wpm"]->get_selected() == "on")
    {
        float info_h = char_dimension['I'].y;
        float info_x = drawer.center, info_y = drawer.get_top_y() - 1.5f * info_h;
        string time_text = " " + convertSeconds((int)round(test_info.time - elapsed), test_info.time) + " ";
        float text_width = MeasureTextEx(time_text, drawer.font_size).x;
        float clock_r = char_dimension['0'].y * 0.4f;

        // DRAW TIME
        // TODO: FIX alignment
        if (!is_tape_mode)
            info_x = drawer.padding + text_width + 2 * clock_r; 
        
        // DRAW TIME
        DrawTextAlign(time_text, info_x, info_y, drawer.font_size, theme.main, RIGHT, CENTER);

        // DRAW TIME CLOCK
        DrawCircleSector(info_x - text_width - clock_r, info_y, clock_r * 0.86f, 180 - (elapsed / test_info.time) * 360, 180, theme.main);
        DrawRing(info_x - text_width - clock_r, info_y, clock_r, clock_r * 0.85f, theme.main);

        // DRAW DASHBOARD 
        float dash_x = info_x + char_dimension[' '].x + clock_r;  // center x of dashboard
        DrawRing(dash_x, info_y, clock_r, clock_r * 0.85f, 45, 315, theme.main);
        DrawCircle(dash_x, info_y, clock_r * 0.2f, theme.main);
        float dash_percent = (cur_wpm / (1.1f * max_wpm + 10));
        const float gap_a = 45;  // 2 * gap_a  = dashboard gap
        float angle = (90 + gap_a) + ((450 - gap_a) - (90 + gap_a)) * dash_percent;
        DrawLineEx({ dash_x - clock_r * 0.4f * cosa(angle), info_y - clock_r * 0.4f * sina(angle) }, { dash_x + clock_r * cosa(angle), info_y + clock_r * sina(angle) }, 2, theme.main);
        // DRAW WPM
        string wpm_text = TextFormat("% 3d", display_wpm);
        DrawTextAlign(wpm_text, dash_x + clock_r, info_y, drawer.font_size, theme.main, LEFT, CENTER);
    }
    // DRAW TYPING TEST TEXT
    drawer.draw();
    
    EndShaderMode();
    if (io_handler[TEST].active_cursor())  // if active or Inactive, blink half of the time
    {
        drawer.draw_caret();
    }
}

void draw_end()
{
    ClearBackground(theme.background);  // Clear render texture background color
    //Vector2 corner = { 0.5f * (gameScreenWidth - (wpm_width + graph_width)), graph_top };
    //DrawRectangle(corner.x, corner.y, wpm_width, graph_height, BLACK);
    //BeginShaderMode(shader);
    //DrawTextAlign(TextFormat("%d wpm", final_wpm), corner.x, corner.y, 75, rgb(32, 32, 32));
    
    //EndShaderMode();
}

void draw_popup()
{
    if (pending_popup_draw)
    {
        DrawRectangle(0, 0, gameScreenWidth, gameScreenHeight, rgba(0, 0, 0, 0.5f));
        pending_popup_draw = false;
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
// Font loading function
void load_base_font(string path = "default")
{
    if (path == "default" || !FileExists(path.c_str()))
    {
        // FONT LOADING USING STATIC CHAR ARRAY
        string full_path = path;
        // SDF font generation from TTF font
        font = { 0 };
        const float font_base = 64;
        font.baseSize = font_base;
        font.glyphCount = 95;
        // Parameters > font size: 16, no glyphs array provided (0), glyphs count: 0 (defaults to 95)
        font.glyphs = LoadFontData(RobotoMono_DATA, RobotoMono_SIZE, font_base, 0, 0, FONT_SDF);
        // Parameters > glyphs count: 95, font size: 16, glyphs padding in image: 0 px, pack method: 1 (Skyline algorythm)
        Image atlas = GenImageFontAtlas(font.glyphs, &font.recs, 95, font_base, 0, 1);
        font.texture = LoadTextureFromImage(atlas);
        UnloadImage(atlas);

        SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);    // Required for SDF font  
    } else 
    {
        font = load_font(path.c_str());
        cout << "Font loaded: " << path << endl;
    }
    font_spacing = 12;  // Default Font 
}

void set_rand_font()
{
    vector<string> font_paths = directory_files("C:/Windows/Fonts", ".ttf");
    string font_path = font_paths[rand_int(0, font_paths.size() - 1)];
    //string font_path = select_file("C:/Users/derek/Documents/projects/typingtest/fonts", "ttf");
    //cout << font_path << endl;
    load_base_font(font_path);
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
    load_base_font("default");
    //set_rand_font();

    init_raw_data;
    for (auto& [key, texture] : textureOf)
    {
        SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
    }

    load_user_data();
    init_settings();

    // STARTING UI ---------------------------------------------------------------
    //new_Toggle(START, 0, 300, 50, true, "test", "settings_icon");
    float bar_h = 25;
    Toggle* test = new Toggle(0, 300, bar_h, true, "test", "settings_icon");
    ToggleGroup* test_group = new ToggleGroup(0, 300, bar_h, 0, { "5", "15", "30", "60", "120" });
    test_group->set_selected(data_json["test time"].as_str());
    //new_ToggleGroup(START, 0, 300, 50, 0, { "15", "30", "60", "120" });
    setting_bar = new SettingBar(gameScreenWidth / 2, 300, { test }, test_group);
    ui_objects.alloc(setting_bar, START);

    
    taskbar = new ToggleGroup(gameScreenWidth/2, gameScreenHeight - 75, 75, 0, { "keyboard", "settings_icon" }, { "default test", "settings" }, true);
    ui_objects.alloc(taskbar, {START, SETTINGS});
    
    // ENDING UI ---------------------------------------------------------------
    new_Button(END, 100, 900, 300, 100, "restart", [] { switch_start(); });
    graph = new Graph(wpm_width + (gameScreenWidth - (graph_width + wpm_width)) * 0.5f, graph_top, graph_width, graph_height, 4), ui_objects.alloc(graph, END);
    end_stats = new TextPanelV(0.5f * (gameScreenWidth - (wpm_width + graph_width)), graph_top, wpm_width, graph_height + 20), ui_objects.alloc(end_stats, END);

    InputBox* input_box = new InputBox(400, 900, 500, 50, "default", false);
    //input_box->set_range(-1000, 1000);
    input_box->set_IOHandler(POPUP);
    ui_objects.alloc(input_box, POPUP);
    
    reset_IOHandler(POPUP);

}


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    
    /**
     * TECHNICAL INITIALIZATION
    */
    // Enable config flags for resizable window and vertical synchro
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT /* | FLAG_WINDOW_UNDECORATED */);

    InitWindow(windowWidth, windowHeight, "CType");
    MaximizeWindow();
    SetWindowMinSize(320, 240);
    set_icon();
    // Render texture initialization, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_TRILINEAR);  // Texture scale filter to use
    
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    SetExitKey(KEY_NULL);
    
    init();
    init_test();

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
    Stopwatch frame_timer;
    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        frame_timer.start();
        //----------------------------------------------------------------------------------
        // Compute required framebuffer scaling
        scale = min((float)GetScreenWidth() / gameScreenWidth, (float)GetScreenHeight() / gameScreenHeight);
        update_mouse();
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        // update alloc 
        ui_objects.update();
        // Update
        globalFrame++;
        update_rect_preview();
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
        if (scene == POPUP)
        {
            
        }
        update_taskbar();
        for (const int id : scene_ids[scene])
        {
            ui_objects[id]->update();
        }
        //shader_on = IsKeyDown(KEY_ENTER);
        //----------------------------------------------------------------------------------
        // Draw everything in the render texture, note this will not be rendered on screen, yet
        BeginTextureMode(target);

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
        }
        
        if (setting_toggle["debug mode"]->get_selected() == "on")
        {
            int max_fps = 1000.0 / frame_time;
            DrawText(TextFormat("MAX FPS: %d | ELAPSED TIME: %.2f", max_fps, frame_time), 0, gameScreenHeight - 30, 25, BLACK);
        }
        
        BeginShaderMode(shader);    // Activate SDF font shader    
        for (const int id : scene_ids[scene])
        {
            ui_objects[id]->draw();
        }
        for (const int id : scene_ids[scene])
        {
            ui_objects[id]->draw_hint();
        }
        draw_rect_preview();
        EndShaderMode();
        draw_logo();
        EndTextureMode();
        // Draw render texture onto real screen ---------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);     // Clear screen background
        //if (shader_on) BeginShaderMode(test_shader);
        // Draw render texture to screen, properly scaled
        DrawTexturePro(target.texture, (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                           (Rectangle){ (GetScreenWidth() - ((float)gameScreenWidth*scale))*0.5f, (GetScreenHeight() - ((float)gameScreenHeight*scale))*0.5f,
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
    //--------------------------------------------------------------------------------------

    return 0;
}