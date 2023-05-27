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
// Init extern variables ------------------------------------------------------------------
// Window Variables
const int windowWidth = 1650, windowHeight = 1000;
int gameScreenWidth = 1920, gameScreenHeight = 1080;
// GLOBAL VARS
unordered_set<int> scene_ids[2];
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

// COLORS
const Color main_color = rgb(27, 27, 27);

UIAlloc UIObjects(100);
Shader shader;
Vector2 char_dimension[CHAR_MAX + 1];

// HANDLERS
TextDrawer drawer;
IOHandler io_handler;
WpmLogger wpm_logger;
// END init extern variables ----------------------------------------------------------------
void start_test()
{
    // reset variables
    word_i = 0;
    empty_i = 0;
    wpm_logger.start();
}

void update()
{
    generate_text(ceil((gameScreenWidth - (drawer.center + drawer.offset)) / char_dimension['i'].x), "english");
    globalFrame++;
    io_handler.update();
    wpm_logger.update();  
}

deque<float> stored_wpm;
deque<float> stored_current_wpm;

void draw_text()
{
    // TEST draw vel
    stored_wpm.push_back(wpm_logger.wpm());
    if (stored_wpm.size() > 1000)
        stored_wpm.pop_front();
    stored_current_wpm.push_back(wpm_logger.current_wpm());
    if (stored_current_wpm.size() > 1000)
        stored_current_wpm.pop_front();
    
    DrawText(TextFormat("%.0f wpm\n%.0f raw", stored_wpm.back(), stored_current_wpm.back()), 0, 0, 30, BLACK);
    const float scale_f = 2;
    for (int x_pos = stored_wpm.size() - 1; x_pos >= 0; x_pos--)
        DrawRectangleRec(formatRect(Rectangle(x_pos, 500 - scale_f*stored_wpm[x_pos], 1, scale_f*stored_wpm[x_pos])), RED);
    for (int x_pos = stored_current_wpm.size() - 1; x_pos >= 0; x_pos--)
        DrawRectangleRec(formatRect(Rectangle(x_pos, 500 - scale_f*stored_current_wpm[x_pos], 1, 10)), BLUE);

    
    drawer.draw();
    if (io_handler.inactive_frames < inactive_time*60 || (io_handler.inactive_frames/30) & 1)  // if active or Inactive, blink half of the time
    {
        drawer.draw_cursor();
    }
}

void init()
{
    // create f# shader file if doesn't exist
    create_shader_file();
    // Load SDF required shader (we use default vertex shader)
    shader = LoadShader(0, shader_path);

    font = LoadFont_Segoeui2(), font_spacing = 15;  // Default Font 
}

void init_test()
{
    drawer = TextDrawer("RobotoMono.ttf", 40);
    io_handler = IOHandler();
    char_status.clear();
    words.clear();
    generated_chars = "";
    generate_text(ceil((gameScreenWidth - (drawer.center + drawer.offset)) / char_dimension['i'].x), "english");
}
Vector2* point_arr;
vector<Vector2> init_points;
int point_count;
void init_bezier()
{
    // bezier test
    BezierPath bezier;
    init_points = vector<Vector2>({ { 50, 800 }, {130, 770}, { 200, 700 }, { 300, 500 }, {400, 600} });
    bezier.Interpolate(init_points, 0.25f);
    auto points = bezier.GetDrawingPoints1();
    point_arr = new Vector2[2*points.size()];
    point_count = 2 * points.size();
    Vector2 previous = points[0];
    Vector2 current = { 0 };
    float thick = 5;
    for (int i = 1; i < points.size(); i++)
    {
        current = points[i];
        float dy = current.y - previous.y;
        float dx = current.x-previous.x;
        float size = 0.5f*thick/sqrtf(dx*dx+dy*dy);

        if (i==1)
        {
            point_arr[0].x = previous.x+dy*size;
            point_arr[0].y = previous.y-dx*size;
            point_arr[1].x = previous.x-dy*size;
            point_arr[1].y = previous.y+dx*size;
        }

        point_arr[2*i+1].x = current.x-dy*size;
        point_arr[2*i+1].y = current.y+dx*size;
        point_arr[2*i].x = current.x+dy*size;
        point_arr[2*i].y = current.y-dx*size;
        previous = current;
    }
}

void draw_curve()
{
    for (auto point : init_points) DrawCircle(point.x, point.y, 5, BLUE);
    //for (int i = 0; i < point_count; i++) DrawCircle(point_arr[i].x, point_arr[i].y, 2, RED);
    DrawTriangleStrip(point_arr, point_count, BLACK);
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
    init_bezier();
    
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
        if (scene == START)
        {
            if (IsKeyPressed())
            {
                start_test();
                scene = TEST;
            }
        }
        if (scene == TEST)
        {
            update();
        }
        
        //----------------------------------------------------------------------------------
        // Draw everything in the render texture, note this will not be rendered on screen, yet
        BeginTextureMode(target);
        ClearBackground(RAYWHITE);  // Clear render texture background color
        int max_fps = 1000.0/frame_time;
        DrawText(TextFormat("MAX FPS: %d | ELAPSED TIME: %.2f", max_fps, frame_time), 0, gameScreenHeight - 30, 25, BLACK);
        draw_text();
        draw_curve();
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
    UnloadShader(shader); 
    CloseWindow();                      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}