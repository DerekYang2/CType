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
// Init extern variables ------------------------------------------------------------------
// Window Variables
const int windowWidth = 1650, windowHeight = 1000;
int gameScreenWidth = 1920, gameScreenHeight = 1080;
// GLOBAL VARS
unordered_set<int> scene_ids[2];
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
// END init extern variables ----------------------------------------------------------------

void update()
{
    globalFrame++;
    io_handler.update();
}

deque<float> stored_vel;
void draw_text()
{
    // TEST draw vel
    stored_vel.push_back(io_handler.offset_vel);
    if (stored_vel.size() > 1000)
        stored_vel.pop_front();
    for (int x_pos = stored_vel.size()-1; x_pos >= 0; x_pos--) 
        DrawRectangleRec(formatRect(Rectangle(x_pos, 100 - 10*stored_vel[x_pos], 1, 10*stored_vel[x_pos])), RED);
    
    drawer.draw();
    if (io_handler.inactive_frames < inactive_time*60 || (io_handler.inactive_frames/30) & 1)  // if active or Inactive, blink half of the time
    {
        drawer.draw_cursor();
    }
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{

    //println(open_file("png,jpg;psd"));

    // Enable config flags for resizable window and vertical synchro
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(windowWidth, windowHeight, "Typing Test");
    SetWindowMinSize(320, 240);

    // Render texture initialization, used to hold the rendering result so we can easily resize it
    RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);  // Texture scale filter to use

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

    create_shader_file();
    // Load SDF required shader (we use default vertex shader)
    shader = LoadShader(0, shader_path);
    
    font = LoadFont_Segoeui(), font_spacing = 15;  // Default Font
    drawer = TextDrawer("RobotoMono.ttf", 40);
    //--------------------------------------------------------------------------------------
    SetExitKey(KEY_NULL);
    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
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
        generate_text(ceil((gameScreenWidth - (drawer.center + drawer.offset)) / char_dimension['i'].x), "english");
        update();
        
        //----------------------------------------------------------------------------------
        // Draw everything in the render texture, note this will not be rendered on screen, yet
        BeginTextureMode(target);
        ClearBackground(RAYWHITE);  // Clear render texture background color
        draw_text();
        DrawFPS(0, 0);
        EndTextureMode();

        // Draw render texture onto real screen
        BeginDrawing();
            ClearBackground(BLACK);     // Clear screen background

            // Draw render texture to screen, properly scaled
            DrawTexturePro(target.texture, (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                           (Rectangle){ (GetScreenWidth() - ((float)gameScreenWidth*scale))*0.5f, (GetScreenHeight() - ((float)gameScreenHeight*scale))*0.5f,
                           (float)gameScreenWidth*scale, (float)gameScreenHeight*scale }, (Vector2){ 0, 0 }, 0.0f, WHITE);
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