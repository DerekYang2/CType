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
#include "segoeui.h"
#include "ObjectMacros.h"
#include "StopWatch.h"
#include "ShaderEmbed.h"
#include "TextGeneration.h"
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

// Init mt19937 with random seed
mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
Font font, line_font; // Font: UI font, line_font: font for the line of text
float font_spacing, line_spacing;

// COLORS
const Color main_color = rgb(27, 27, 27);

UIAlloc UIObjects(100);

// END init extern variables ----------------------------------------------------------------
// Main variables
int inactive_frames = 0;
int back_frames = 0;
Shader shader;
float offset_x = 0;
float line_font_size = 50;
int future_draw = 0;
float center_x = gameScreenWidth / 2;
float offset_vel = 0, vel_target = 0;

Vector2 char_dimension[CHAR_MAX + 1];

void load_char_dimensions()
{
    for (char c = ' '; c <= '~'; c++)
    {
        string str(1, c);
        char_dimension[c] = MeasureTextEx(line_font, str.c_str(), line_font_size, line_font_size / line_spacing);
    }
}
bool handled_press[CHAR_MAX + 1];

void update()
{
    globalFrame++;
    int key_pressed = GetKeyPressed();
    fill(handled_press, handled_press + CHAR_MAX + 1, false);
    while (key_pressed)  // IF PRESSED
    {
        char char_pressed = convertKey(key_pressed); 
        // shift the character pressed
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
            char_pressed = shiftChar(char_pressed);
        if (char_pressed != 0)  // valid character
        {
            char status = CORRECT;
            char_status.push_back({ status, char_pressed });
            offset_x += char_dimension[char_pressed].x;
            empty_i++;
            handled_press[char_pressed] = true;  // Do not handle it again
        } else if (key_pressed == KEY_BACKSPACE)
        {
            if (empty_i > 0)
            {
                offset_x -= char_dimension[char_status.back().second].x;
                char_status.pop_back();
                empty_i--;
            }
        } else if (key_pressed == KEY_SPACE)  // requires special handling
        {
            char status = CORRECT;
            char_status.push_back({ status, ' ' });
            offset_x += char_dimension[' '].x;
            empty_i++;
            handled_press[' '] = true;
        }
        key_pressed = GetKeyPressed();
    }

    // HANDLE held char
    char char_pressed = (char)GetCharPressed();
    while (char_pressed)
    {
        if (!handled_press[char_pressed])
        {
            char status = CORRECT;
            char_status.push_back({ status, char_pressed });
            offset_x += char_dimension[char_pressed].x;
            empty_i++;
        }
        char_pressed = (char)GetCharPressed();
    } 

    // Handle held backspace
    if (IsKeyDown(KEY_BACKSPACE))
        back_frames++;
    else
        back_frames = 0;
    
    if (!IsKeyPressed(KEY_BACKSPACE) && (back_frames > 30 && empty_i > 0))  // IF HELD
    {
        offset_x -= char_dimension[char_status.back().second].x;
        char_status.pop_back();
        empty_i--;
    }
     
    if (GetKeyPressed() == KEY_NULL)
        inactive_frames++;
    else
        inactive_frames = 0;

    //cout << offset_x << ' ';
    // update offset 
    float wpm = 120;
    float secperchar = 1.0 / (wpm * 5 / 60);  // predicted seconds to type a char
    vel_target = offset_x / max(1.f, (secperchar * 60));  // cannot divide by anything smaller than 1
    offset_vel += (vel_target - offset_vel) * 0.3f;
    offset_x -= offset_vel;
}

void load_font(string file_name, float spacing = 15)
{
    string full_path = "fonts/" + file_name; 
    //line_font = LoadFontEx(full_path.c_str(), 256, NULL, 0);
    line_spacing = spacing;
    
    // Loading file to memory
    unsigned int fileSize = 0;
    unsigned char* fileData = LoadFileData(full_path.c_str(), &fileSize);
    
    // SDF font generation from TTF font
    line_font = { 0 };
    const float font_base = 64;
    line_font.baseSize = font_base;
    line_font.glyphCount = 95;
    // Parameters > font size: 16, no glyphs array provided (0), glyphs count: 0 (defaults to 95)
    line_font.glyphs = LoadFontData(fileData, fileSize, font_base, 0, 0, FONT_SDF);
    // Parameters > glyphs count: 95, font size: 16, glyphs padding in image: 0 px, pack method: 1 (Skyline algorythm)
    Image atlas = GenImageFontAtlas(line_font.glyphs, &line_font.recs, 95, font_base, 0, 1);
    line_font.texture = LoadTextureFromImage(atlas);
    UnloadImage(atlas);
    UnloadFileData(fileData);      // Free memory from loaded file

    // Load SDF required shader (we use default vertex shader)
    shader = LoadShader(0, shader_path);
    SetTextureFilter(line_font.texture, TEXTURE_FILTER_BILINEAR);    // Required for SDF font
}
deque<float> stored_vel;
void draw_text()
{
    float cursor_h = char_dimension['I'].y;
    float bottom_y = (gameScreenHeight + cursor_h) / 2;
    float left_most = center_x + offset_x;
    float font_size = line_font_size;
    DrawLine(0, bottom_y, gameScreenWidth, bottom_y, GREEN);

    // TEST draw vel
    stored_vel.push_back(offset_vel);
    if (stored_vel.size() > 1000)
        stored_vel.pop_front();
    for (int x_pos = stored_vel.size()-1; x_pos >= 0; x_pos--) 
        DrawRectangleRec(formatRect(Rectangle(x_pos, 100 - 5*stored_vel[x_pos], 1, 5*stored_vel[x_pos])), RED);
    
    BeginShaderMode(shader);    // Activate SDF font shader    
    for (int i = char_status.size() - 1; i >= 0 && left_most >= 0; i--)
    {
        string char_str(1, char_status[i].second);
        Vector2 dimension = char_dimension[char_status[i].second];
        left_most -= dimension.x;
        DrawTextEx(line_font, char_str.c_str(), { left_most, bottom_y - dimension.y}, font_size, font_size / line_spacing, BLUE);
    }
    float right_most = center_x + offset_x;
    for (int i = empty_i; i < generated_chars.size() && right_most + 2*char_dimension['w'].x <= gameScreenWidth; i++)
    {
        string char_str(1, generated_chars[i]);
        Vector2 dimension = char_dimension[generated_chars[i]];
        DrawTextEx(line_font, char_str.c_str(), { right_most, bottom_y - dimension.y}, font_size, font_size / line_spacing, RED);
        right_most += dimension.x;
    }
    EndShaderMode();
    if (inactive_frames <= 60 * 2)  // If was typing in last 2 seconds 
    {
        // Draw Cursor
        DrawRectangleRounded(Rectangle(center_x, bottom_y - cursor_h, 3, cursor_h), 0.8f, 7, BLACK);
    } else if ((inactive_frames/30) & 1)  // Inactive, blink half of the time
    {
        // Draw Cursor
        DrawRectangleRounded(Rectangle(center_x, bottom_y - cursor_h, 3, cursor_h), 0.8f, 7, BLACK);
    }
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{

    //println(open_file("png,jpg;psd"));

    // Enable config flags for resizable window and vertical synchro
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(windowWidth, windowHeight, "Typing Test");
    SetWindowMinSize(320, 240);

    // Render texture initialization, used to hold the rendering result so we can easily resize it
    RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);  // Texture scale filter to use

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

    create_shader_file();
    
    font = LoadFont_Segoeui(), font_spacing = 15;  // Default Font
    load_font("RobotoMono.ttf");
    load_char_dimensions();
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
        generate_text(ceil((gameScreenWidth - (center_x + offset_x)) / char_dimension['i'].x), "english");
        update();
        
        //----------------------------------------------------------------------------------
        // Draw everything in the render texture, note this will not be rendered on screen, yet
        BeginTextureMode(target);
        ClearBackground(RAYWHITE);  // Clear render texture background color
        draw_text();
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
    UnloadFont(line_font);
    UnloadShader(shader); 
    CloseWindow();                      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}