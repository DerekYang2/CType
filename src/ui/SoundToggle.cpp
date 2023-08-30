#include <filesystem>
#include "SoundToggle.h"

const int SOUND_PER_ROW = 4;

SoundToggle::SoundToggle(float x, float y, float w, float h, string init_folder) : ToggleGroup(x, y, h, 0, { "anything" }, true)
{
    namespace fs = std::filesystem;
    tot_width = w;
    text.clear();

    sample.push_back(Sound());
    text.push_back("off");
    // Load sample sound from each folder
    for (const auto& path : fs::directory_iterator(SOUNDS_FOLDER))
    {
        if (fs::is_directory(path))
        {
            vector<string> wav_paths = directory_files(path.path().string(), ".wav");
            string first_sound = wav_paths[0];
            replace(first_sound.begin(), first_sound.end(), '\\', '/');  // Be consistent with backslash direction
            sample.push_back(LoadSoundFromWave(LoadWaveFromMemory(".wav", loaded_file_data[first_sound].first, loaded_file_data[first_sound].second)));
            SetSoundVolume(sample.back(), 1.0f);
            text.push_back(path.path().filename().string());

            string formatted = text.back();
            replace(formatted.begin(), formatted.end(), '_', ' ');
        }
    }

    for (int i = 0; i < text.size(); i++)
    {
        if (text[i] == init_folder)
            selected = i;
    }
    
    font_size = MeasureFontSize("A", INT_MAX, h / 1.5f);

    hitbox.clear();
    for (int i = 0; i < text.size(); i++)
        hitbox.push_back({ 0, 0, 0, h });

    padding = h / 5;
    float rect_w = (tot_width - (SOUND_PER_ROW - 1) * padding) / (SOUND_PER_ROW);
    for (int i = 0; i < hitbox.size(); i++)
    {
        int col = i % SOUND_PER_ROW, row = i / SOUND_PER_ROW;
        hitbox[i].x = corner.x + col * (padding + rect_w);
        hitbox[i].y = corner.y + row * h + (row + 1) * padding;
        hitbox[i].width = rect_w;
    }
}

void SoundToggle::draw()
{
    const float stroke_w = 3.f;

    vector<int> index_order;
    int hover_idx = -1;
    for (int i = 0; i < hitbox.size(); i++)
    {
        if (i != selected)
            index_order.push_back(i);
        if (hover[i])
            hover_idx = i;
    }

    if (hover_idx != -1)
        index_order.push_back(hover_idx);
    index_order.push_back(selected);

    for (int i : index_order)
    {
        Color rect_col, text_col;
        if (hover[i])
        {
            rect_col = theme.text;
            text_col = theme.background;
        } else if (i == selected)
        {
            rect_col = theme.main;
            text_col = theme.background;
        } else
        {
            rect_col = theme.sub_alt;
            text_col = theme.text;
        }
        bool pressed = (i == selected && pressWatch.s() < TOGGLE_DELAY);
        if (pressed)
            rect_col = theme.sub;  // blink sub when click
        float border = (i == selected || hover[i]) ? stroke_w : 0;
        DrawRectangleRoundedAlign(hitbox[i].x + hitbox[i].width * 0.5f, hitbox[i].y + hitbox[i].height * 0.5f, hitbox[i].width + 2 * border + 2 * pressed, hitbox[i].height + 2 * border + 2 * pressed, 0.35f, roundedSegments(hitbox[i].height), rect_col, CENTER, CENTER);

        string formatted_text = text[i];
        replace(formatted_text.begin(), formatted_text.end(), '_', ' ');
        DrawTextAlign(formatted_text, hitbox[i].x + hitbox[i].width * 0.5f, hitbox[i].y + hitbox[i].height * 0.5f, font_size, text_col, CENTER, CENTER);
    }
}

void SoundToggle::update()
{
    ToggleGroup::update();
    if (was_pressed())
    {
        if (get_selected() == "off") // off has no sound
            return;
        PlaySound(sample[selected]);
    }
}

Rectangle SoundToggle::bounding_box()
{
    return Rectangle(corner.x, corner.y, tot_width, get_height());
}

string SoundToggle::get_selected()
{
    return text[selected];
}

float SoundToggle::get_width()
{
    return tot_width;
}

float SoundToggle::get_height()
{
    float h = hitbox[0].height;
    float padding = h / 5;
    int tot_rows = ceil_div(hitbox.size(), SOUND_PER_ROW);
    return tot_rows * h + (tot_rows + 1) * padding;
}