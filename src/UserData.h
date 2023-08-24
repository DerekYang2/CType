#pragma once
#include "UIAlloc.h"
#include "jsonparser.h"
/**
 * TODO: add test history and user statistics saving
*/

// EXTERNALS
extern UIAlloc ui_objects;
//
extern string DATA_PATH;
extern RSJresource data_json;
extern void load_user_data();
extern void write_user_data();
