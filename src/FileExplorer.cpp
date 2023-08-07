#include "FileExplorer.h"
#include <windows.h>
#include <shellapi.h>

void open_path(std::string path)
{
    ShellExecuteA(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
}
