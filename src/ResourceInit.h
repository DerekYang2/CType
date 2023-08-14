#pragma once
#include "at_icon.h"
#include "exit_icon.h"
#include "fullscreen.h"
#include "globe.h"
#include "hashtag_icon.h"
#include "keyboard.h"
#include "logo.h"
#include "logo_icon.h"
#include "minimise_icon.h"
#include "reload.h"
#include "settings_icon.h"
#include "unfullscreen.h"

#define init_raw_data \
Image _img = {0}; \
_img = { 0 }; \
_img.format = AT_ICON_FORMAT; \
_img.width = AT_ICON_WIDTH; \
_img.height = AT_ICON_HEIGHT; \
_img.data = AT_ICON_DATA; \
_img.mipmaps = 1; \
textureOf["at_icon"] = LoadTextureFromImage(_img); \
_img = { 0 }; \
_img.format = EXIT_ICON_FORMAT; \
_img.width = EXIT_ICON_WIDTH; \
_img.height = EXIT_ICON_HEIGHT; \
_img.data = EXIT_ICON_DATA; \
_img.mipmaps = 1; \
textureOf["exit_icon"] = LoadTextureFromImage(_img); \
_img = { 0 }; \
_img.format = FULLSCREEN_FORMAT; \
_img.width = FULLSCREEN_WIDTH; \
_img.height = FULLSCREEN_HEIGHT; \
_img.data = FULLSCREEN_DATA; \
_img.mipmaps = 1; \
textureOf["fullscreen"] = LoadTextureFromImage(_img); \
_img = { 0 }; \
_img.format = GLOBE_FORMAT; \
_img.width = GLOBE_WIDTH; \
_img.height = GLOBE_HEIGHT; \
_img.data = GLOBE_DATA; \
_img.mipmaps = 1; \
textureOf["globe"] = LoadTextureFromImage(_img); \
_img = { 0 }; \
_img.format = HASHTAG_ICON_FORMAT; \
_img.width = HASHTAG_ICON_WIDTH; \
_img.height = HASHTAG_ICON_HEIGHT; \
_img.data = HASHTAG_ICON_DATA; \
_img.mipmaps = 1; \
textureOf["hashtag_icon"] = LoadTextureFromImage(_img); \
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
_img.format = MINIMISE_ICON_FORMAT; \
_img.width = MINIMISE_ICON_WIDTH; \
_img.height = MINIMISE_ICON_HEIGHT; \
_img.data = MINIMISE_ICON_DATA; \
_img.mipmaps = 1; \
textureOf["minimise_icon"] = LoadTextureFromImage(_img); \
_img = { 0 }; \
_img.format = RELOAD_FORMAT; \
_img.width = RELOAD_WIDTH; \
_img.height = RELOAD_HEIGHT; \
_img.data = RELOAD_DATA; \
_img.mipmaps = 1; \
textureOf["reload"] = LoadTextureFromImage(_img); \
_img = { 0 }; \
_img.format = SETTINGS_ICON_FORMAT; \
_img.width = SETTINGS_ICON_WIDTH; \
_img.height = SETTINGS_ICON_HEIGHT; \
_img.data = SETTINGS_ICON_DATA; \
_img.mipmaps = 1; \
textureOf["settings_icon"] = LoadTextureFromImage(_img); \
_img = { 0 }; \
_img.format = UNFULLSCREEN_FORMAT; \
_img.width = UNFULLSCREEN_WIDTH; \
_img.height = UNFULLSCREEN_HEIGHT; \
_img.data = UNFULLSCREEN_DATA; \
_img.mipmaps = 1; \
textureOf["unfullscreen"] = LoadTextureFromImage(_img); \
;

