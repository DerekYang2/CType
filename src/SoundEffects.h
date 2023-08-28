#pragma once
#include "globals.h"
extern const string SOUNDS_FOLDER;
extern string loaded_sound_path;
extern vector<Sound> loaded_sfx;
extern Sound error_sound;

extern void load_sound_effects(string folder_path);
extern void play_sound();
extern void play_error_sound();
extern void set_error_volume(float volume);