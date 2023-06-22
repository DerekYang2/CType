#pragma once
#include <string>
#include "nfd.h"
std::string select_file(std::string filter);
std::string select_file(std::string default_path, std::string filter);