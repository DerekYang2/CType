#pragma once 
typedef struct StatusCount {
    int correct, incorrect, missing, extra;
} StatusCount;

extern StatusCount status_count;
void update_status(char c);
void update_space();
void update_backspace();