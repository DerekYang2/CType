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
#include "UIAlloc.h"
#include "Button.h"
#include "segoeui.h"
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
#include "SettingBar.h"
#include "TestInfo.h"
#include "RectPreview.h"
// Init extern variables ------------------------------------------------------------------
Theme theme{
    WHITE, // background
    rgb(14, 105, 161), // main
    rgb(61, 78, 92), // sub 
    BLACK, // text 
    rgb(255, 0, 0), // error
    rgb(128, 41, 41) // error_extra
};
// Window Variables
const int windowWidth = 1650, windowHeight = 1000;
int gameScreenWidth = 1920, gameScreenHeight = 1080;
// GLOBAL VARS
unordered_set<int> scene_ids[5];
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

UIAlloc ui_objects(25);
Shader shader;
Vector2 char_dimension[CHAR_MAX + 1];

// HANDLERS
TextDrawer drawer;
IOHandler io_handler;
WpmLogger wpm_logger;
TestInfo test_info;
TextGenerator text_gen;

bool drawing_block;
float drawing_x, drawing_y;
// END init extern variables ----------------------------------------------------------------
int final_wpm;
float elapsed;
Graph* graph;
SettingBar* setting_bar;
// test display vars
int display_wpm_frames = 15;  // frames to update wpm display
int display_wpm;
float max_wpm = 0, cur_wpm = 0;
// UI DIMENSIONS 
float wpm_width = 300;  // width of wpm text 
float graph_width = 1400;
float graph_top = 200;
float graph_height = 600;

void init_test()
{
    drawer = TextDrawer(font, 40);
    io_handler = IOHandler();
    char_status.clear();
    words.clear();
    // reset variables
    elapsed = 0;
    word_i = 0;
    empty_i = 0;
    wpm_logger.reset();
    // generate first chars
    generated_chars = "";
    text_gen.set_list("english");
    text_gen.generate_text(ceil((gameScreenWidth - (drawer.center + drawer.offset)) / char_dimension['i'].x));
}

void start_test()
{
    test_info.init(stoi(setting_bar->group_selected()));
    wpm_logger.start();
    scene = TEST;
    display_wpm = 0;
    cur_wpm = 0;
    max_wpm = 0;
}

void end_test()
{
    wpm_logger.end();
    final_wpm = round(wpm_logger.wpm());
    scene = END;
    graph->reset();
    graph->set_time(elapsed);
    graph->config_max(test_info.wpm_record);
    graph->config_max(test_info.raw_wpm_record, RAW);
    graph->set_plot(test_info.wpm_record);
    graph->set_plot(test_info.raw_wpm_record, RAW);
}

void switch_start()
{
    scene = START;
    init_test(); 
}

void update_start()
{
    if (IsKeyPressed(KEY_TAB))  // restart test 
    {
        init_test();
        return;
    }
    if (IsKeyPressed())
    {
        start_test();
    }
} 

void update_test()
{
    text_gen.generate_text(ceil((gameScreenWidth - (drawer.center + drawer.offset)) / char_dimension['i'].x));
    io_handler.update();
    wpm_logger.update();
    test_info.update();
    elapsed = wpm_logger.elapsed();

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
    }
}

void update_end()
{
    
}

void draw_start()
{
    BeginShaderMode(shader);
    drawer.draw();
    if (io_handler.inactive_frames < inactive_time*60 || (io_handler.inactive_frames/30) & 1)  // if active or Inactive, blink half of the time
    {
        drawer.draw_cursor();
    }
    EndShaderMode();
}

/* deque<float> stored_wpm;
deque<float> stored_current_wpm; */

void draw_test()
{
    // TEST draw vel
/*     stored_wpm.push_back(wpm_logger.wpm());
    if (stored_wpm.size() > 1000)
        stored_wpm.pop_front();
    stored_current_wpm.push_back(wpm_logger.raw_wpm());
    if (stored_current_wpm.size() > 1000)
        stored_current_wpm.pop_front(); */
    
/*     const float scale_f = 2;
    for (int x_pos = stored_wpm.size() - 1; x_pos >= 0; x_pos--)
        DrawRectangleRec(formatRect(Rectangle(x_pos, 500 - scale_f*stored_wpm[x_pos], 1, scale_f*stored_wpm[x_pos])), RED);
    for (int x_pos = stored_current_wpm.size() - 1; x_pos >= 0; x_pos--)
        DrawRectangleRec(formatRect(Rectangle(x_pos, 500 - scale_f*stored_current_wpm[x_pos], 1, 10)), BLUE); */
    BeginShaderMode(shader);
    
    float info_h = char_dimension['I'].y;
    float info_x = drawer.center, info_y = drawer.get_top_y() - 1.5f * info_h;
    string time_text = " " + convertSeconds((int)round(test_info.time - elapsed), test_info.time) + " ";
    float text_width = MeasureTextEx(time_text, drawer.font_size).x;
    // DRAW TIME
    DrawTextAlign(time_text, info_x, info_y, drawer.font_size, theme.text, RIGHT, CENTER);

    // DRAW TIME CLOCK
    float clock_r = char_dimension['0'].y * 0.4f;
    DrawCircleSector(info_x - text_width - clock_r, info_y, clock_r * 0.86f, 180 - (elapsed / test_info.time) * 360, 180, theme.text);
    DrawRing(info_x - text_width - clock_r, info_y, clock_r, clock_r * 0.85f, theme.text);

    // DRAW DASHBOARD 
    float dash_x = info_x + char_dimension[' '].x + clock_r;  // center x of dashboard
    DrawRing(dash_x, info_y, clock_r, clock_r * 0.85f, 45, 315, theme.text);
    DrawCircle(dash_x, info_y, clock_r * 0.2f, theme.text);
    float dash_percent = (cur_wpm / (1.1f * max_wpm + 10));
    const float gap_a = 45;  // 2 * gap_a  = dashboard gap
    float angle = (90 + gap_a) + ((450 - gap_a) - (90 + gap_a)) * dash_percent;
    DrawLineEx({ dash_x - clock_r * 0.4f * cosa(angle), info_y - clock_r * 0.4f * sina(angle) }, { dash_x + clock_r * cosa(angle), info_y + clock_r * sina(angle) }, 2, theme.text);
    // DRAW WPM
    string wpm_text = TextFormat("% 3d", display_wpm);
    DrawTextAlign(wpm_text, dash_x + clock_r, info_y, drawer.font_size, theme.text, LEFT, CENTER);
    
    // DRAW TYPING TEST TEXT
    drawer.draw();
    
    EndShaderMode();
    if (io_handler.inactive_frames < inactive_time * 60 || (io_handler.inactive_frames / 30) & 1)  // if active or Inactive, blink half of the time
    {
        drawer.draw_cursor();
    }
}

void draw_end()
{
    Vector2 corner = { 0.5f * (gameScreenWidth - (wpm_width + graph_width)), graph_top };
    DrawRectangle(corner.x, corner.y, wpm_width, graph_height, BLACK);
    BeginShaderMode(shader);
    DrawTextAlign(TextFormat("%d wpm", final_wpm), corner.x, corner.y, 75, WHITE);
    EndShaderMode();
}

// Font loading function: Segoeui
void load_base_font(string path = "C:/Windows/Fonts/segoeui.ttf")
{
    if (FileExists(path.c_str()))
    {
        font = load_font(path.c_str());
        cout << "Font loaded: " << path << endl;
    } else
    {
        font = { 0 };

        font.baseSize = 256;
        font.glyphCount = 95;
        font.glyphPadding = 0;

        // Custom font loading
        // NOTE: Compressed font image data (DEFLATE), it requires DecompressData() function
        int fontDataSize_Segoeui = 0;
        unsigned char* data = DecompressData(fontData_Segoeui, COMPRESSED_DATA_SIZE_FONT_SEGOEUI, &fontDataSize_Segoeui);
        Image imFont = { data, 4096, 4096, 1, 2 };

        // Load texture from image
        font.texture = LoadTextureFromImage(imFont);
        SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

        UnloadImage(imFont);  // Uncompressed data can be unloaded from memory

        // Assign glyph recs and info data directly
        // WARNING: This font data must not be unloaded
        font.recs = fontRecs_Segoeui;
        font.glyphs = fontGlyphs_Segoeui;
    }
    font_spacing = 15;  // Default Font 
}

void init()
{
    // create f# shader file if doesn't exist
    create_shader_file();
    // Load SDF required shader (we use default vertex shader)
    shader = LoadShader(0, shader_path);
    //test_shader = LoadShader(0, "./fonts/test.fs");
    load_base_font("./fonts/RobotoMono.ttf");
    init_raw_data;
    new_Button(END, 100, 900, 300, 100, "restart", [] { switch_start(); });
    //new_Toggle(START, 0, 300, 50, true, "test", "settings_icon");
    
    Toggle* test = new Toggle(0, 300, 50, true, "test", "settings_icon");
    ToggleGroup* test_group = new ToggleGroup(0, 300, 50, 0, { "15", "30", "60", "120" });
    //new_ToggleGroup(START, 0, 300, 50, 0, { "15", "30", "60", "120" });
    setting_bar = new SettingBar(gameScreenWidth / 2, 300, { test }, test_group);
    ui_objects.alloc(setting_bar, START);
    
    graph = new Graph(wpm_width + (gameScreenWidth - (graph_width + wpm_width)) * 0.5f, graph_top, graph_width, graph_height, 4);
    ui_objects.alloc(graph, END);
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
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(windowWidth, windowHeight, "Typing Test");
    SetWindowMinSize(320, 240);
    // Render texture initialization, used to hold the rendering result so we can easily resize it
    RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);  // Texture scale filter to use
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    SetExitKey(KEY_NULL);
    
    init();
    init_test();
    
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
        float scale = min((float)GetScreenWidth()/gameScreenWidth, (float)GetScreenHeight()/gameScreenHeight);

        // Update virtual mouse (clamped mouse value behind game screen)
        Vector2 real_mouse = GetMousePosition();
        mouse.x = (real_mouse.x - (GetScreenWidth() - (gameScreenWidth*scale))*0.5f)/scale;
        mouse.y = (real_mouse.y - (GetScreenHeight() - (gameScreenHeight*scale))*0.5f)/scale;
        mouse = Vector2Clamp(mouse, (Vector2){ 0, 0 }, (Vector2){ (float)gameScreenWidth, (float)gameScreenHeight });

        //----------------------------------------------------------------------------------
        // Update
        globalFrame++;
        update_rect_preview();
        // must be if instead of of else if so start transitions into test directly
        if (scene == START)
        {
            update_start();
        }
        if (scene == TEST)
        {
            update_test();
        }
        if (scene == END)
        {
            update_end();
        }

        for (const int id : scene_ids[scene])
        {
            ui_objects[id]->update();
        }
        //----------------------------------------------------------------------------------
        // Draw everything in the render texture, note this will not be rendered on screen, yet
        BeginTextureMode(target);
        ClearBackground(RAYWHITE);  // Clear render texture background color
        int max_fps = 1000.0 / frame_time;
        DrawText(TextFormat("MAX FPS: %d | ELAPSED TIME: %.2f", max_fps, frame_time), 0, gameScreenHeight - 30, 25, BLACK);
        if (scene == START)
        {
            draw_start();
        } else if (scene == TEST)
        {
            draw_test();
        } else if (scene == END)
        {
            draw_end();
        }

        BeginShaderMode(shader);    // Activate SDF font shader    
        for (const int id : scene_ids[scene])
        {
            ui_objects[id]->draw();
        }
        draw_rect_preview();
        EndShaderMode();
        EndTextureMode();

        // Draw render texture onto real screen
        BeginDrawing();
            ClearBackground(BLACK);     // Clear screen background

            // Draw render texture to screen, properly scaled
            DrawTexturePro(target.texture, (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                           (Rectangle){ (GetScreenWidth() - ((float)gameScreenWidth*scale))*0.5f, (GetScreenHeight() - ((float)gameScreenHeight*scale))*0.5f,
                           (float)gameScreenWidth*scale, (float)gameScreenHeight*scale }, (Vector2){ 0, 0 }, 0.0f, WHITE);
        if (globalFrame % 30 == 0)
            frame_time = frame_timer.ms();
        EndDrawing();
        //--------------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(target);        // Unload render texture
    for (auto& [path, texture] : textureOf)
        UnloadTexture(texture);
    UnloadShader(shader);
    CloseWindow();                      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}