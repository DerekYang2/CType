#include <filesystem>
#include "SoundEffects.h"

string loaded_sound_path = "";
const string SOUNDS_FOLDER = "sounds";
vector<Sound> loaded_sfx;
Sound error_sound;

void load_sound_effects(string folder_path)
{
    if (folder_path == loaded_sound_path)
        return;
    loaded_sound_path = folder_path;
    // Load error sound if unloaded
    if (!IsSoundReady(error_sound))
    {
        string error_path = SOUNDS_FOLDER + "/error.wav";
        error_sound = LoadSound(error_path.c_str());
    }

    for (Sound& sound : loaded_sfx)
        UnloadSound(sound);
    loaded_sfx.clear();
    
    vector<string> wav_paths = directory_files(folder_path, ".wav");
    for (string file_path : wav_paths)
    {
        replace(file_path.begin(), file_path.end(), '\\', '/');  // Be consistent with backslash direction
        loaded_sfx.push_back(LoadSoundFromWave(LoadWaveFromMemory(".wav", loaded_file_data[file_path].first, loaded_file_data[file_path].second)));
    }
}

void play_sound()
{
    // Play a random sound in the loaded_sfx vector
    PlaySound(loaded_sfx[rand_int(0, loaded_sfx.size() - 1)]);
}

void play_error_sound()
{
    PlaySound(error_sound);
}

void set_error_volume(float volume)
{
    SetSoundVolume(error_sound, clamp(volume, 0.0f, 1.0f));
}