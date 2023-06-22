#pragma once
#include "arrow_cursor.h"
#include "keyboard.h"
#include "link_cursor.h"
#include "settings_icon.h"

#define init_raw_data \
Image _img = {0}; \
_img = { 0 }; \
_img.format = ARROW_CURSOR_FORMAT; \
_img.width = ARROW_CURSOR_WIDTH; \
_img.height = ARROW_CURSOR_HEIGHT; \
_img.data = ARROW_CURSOR_DATA; \
_img.mipmaps = 1; \
textureOf["arrow_cursor"] = LoadTextureFromImage(_img); \
_img = { 0 }; \
_img.format = KEYBOARD_FORMAT; \
_img.width = KEYBOARD_WIDTH; \
_img.height = KEYBOARD_HEIGHT; \
_img.data = KEYBOARD_DATA; \
_img.mipmaps = 1; \
textureOf["keyboard"] = LoadTextureFromImage(_img); \
_img = { 0 }; \
_img.format = LINK_CURSOR_FORMAT; \
_img.width = LINK_CURSOR_WIDTH; \
_img.height = LINK_CURSOR_HEIGHT; \
_img.data = LINK_CURSOR_DATA; \
_img.mipmaps = 1; \
textureOf["link_cursor"] = LoadTextureFromImage(_img); \
_img = { 0 }; \
_img.format = SETTINGS_ICON_FORMAT; \
_img.width = SETTINGS_ICON_WIDTH; \
_img.height = SETTINGS_ICON_HEIGHT; \
_img.data = SETTINGS_ICON_DATA; \
_img.mipmaps = 1; \
textureOf["settings_icon"] = LoadTextureFromImage(_img); \
;

