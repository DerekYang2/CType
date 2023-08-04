#include "Utils.h"

class FontMeasure {
    public:
    Font font;
    float small_height, small_size;
    float medium_height, medium_size;
    float large_height, large_size;
    float title_height, title_size;
    FontMeasure(float small_height, float medium_height, float large_height, float title_height);
    FontMeasure(Font f, float small_height, float medium_height, float large_height, float title_height);
    void set_font(Font f);
    float small();
    float medium();
    float large();
    float title();
};