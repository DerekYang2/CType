#pragma once
#include "settings_icon.h"

#define init_raw_data \
Image _img = {0}; \
_img = { 0 }; \
_img.format = SETTINGS_ICON_FORMAT; \
_img.width = SETTINGS_ICON_WIDTH; \
_img.height = SETTINGS_ICON_HEIGHT; \
_img.data = SETTINGS_ICON_DATA; \
_img.mipmaps = 1; \
textureOf["settings_icon"] = LoadTextureFromImage(_img); \
;

