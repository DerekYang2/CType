#include "nfd_wrapper.h"
std::string open_file(std::string filter)
{
    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_OpenDialog( filter.c_str(), NULL, &outPath );
    if ( result == NFD_OKAY )
    {
        return std::string(outPath);
    }
    else if ( result == NFD_CANCEL )
    {
        return "!C";
    }
    else 
    {
        return "!E:" + std::string(NFD_GetError());
    }
}