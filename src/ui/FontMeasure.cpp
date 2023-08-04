#include "FontMeasure.h"


FontMeasure::FontMeasure(float small_height, float medium_height, float large_height, float title_height) : small_height(small_height), medium_height(medium_height), large_height(large_height), title_height(title_height)
{
}

FontMeasure::FontMeasure(Font f, float small_height, float medium_height, float large_height, float title_height) : small_height(small_height), medium_height(medium_height), large_height(large_height), title_height(title_height)
{
    set_font(f);
}

void FontMeasure::set_font(Font f)
{
    font = f;
    small_size = MeasureFontSize("I", INT_MAX, small_height);
    medium_size = MeasureFontSize("I", INT_MAX, medium_height);
    large_size = MeasureFontSize("I", INT_MAX, large_height);
    title_size = MeasureFontSize("I", INT_MAX, title_height);
}

float FontMeasure::small()
{
    return small_size;
}

float FontMeasure::medium()
{
    return medium_size;
}

float FontMeasure::large()
{
    return large_size;
}

float FontMeasure::title()
{
    return title_size;
}