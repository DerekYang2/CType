#pragma once
#include "keyboard.h"
#include "logo.h"
#include "logo_icon.h"
#include "settings_icon.h"

#define init_raw_data \
Image _img = {0}; \
_img = { 0 }; \
_img.format = KEYBOARD_FORMAT; \
_img.width = KEYBOARD_WIDTH; \
_img.height = KEYBOARD_HEIGHT; \
_img.data = KEYBOARD_DATA; \
_img.mipmaps = 1; \
textureOf["keyboard"] = LoadTextureFromImage(_img); \
_img = { 0 }; \
_img.format = LOGO_FORMAT; \
_img.width = LOGO_WIDTH; \
_img.height = LOGO_HEIGHT; \
_img.data = LOGO_DATA; \
_img.mipmaps = 1; \
textureOf["logo"] = LoadTextureFromImage(_img); \
_img = { 0 }; \
_img.format = LOGO_ICON_FORMAT; \
_img.width = LOGO_ICON_WIDTH; \
_img.height = LOGO_ICON_HEIGHT; \
_img.data = LOGO_ICON_DATA; \
_img.mipmaps = 1; \
textureOf["logo_icon"] = LoadTextureFromImage(_img); \
_img = { 0 }; \
_img.format = SETTINGS_ICON_FORMAT; \
_img.width = SETTINGS_ICON_WIDTH; \
_img.height = SETTINGS_ICON_HEIGHT; \
_img.data = SETTINGS_ICON_DATA; \
_img.mipmaps = 1; \
textureOf["settings_icon"] = LoadTextureFromImage(_img); \
;

