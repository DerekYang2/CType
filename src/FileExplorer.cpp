#include "FileExplorer.h"

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#elif __linux__
// ???
#elif __APPLE__ || __MACH__
// ???
#endif 

void open_path(std::string path)
{
    #ifdef _WIN32
    ShellExecuteA(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
    #endif
}
