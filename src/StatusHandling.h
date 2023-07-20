#pragma once 
#include "globals.h"
#include "IOHandler.h"
#include "TextDrawer.h"

typedef struct StatusCount {
    int correct, incorrect, missing, extra;
} StatusCount;

extern StatusCount status_count;
void update_status(char c);
void update_space();
void update_backspace();