#include "Theme.h"

Theme::Theme(Color background, Color main, Color caret, Color sub, Color text, Color error, Color error_extra) : background(background), main(main), caret(caret), sub(sub), text(text), error(error), error_extra(error_extra)
{
    background_shade = Color(background.r * (1.0f - 0.15f), background.g * (1.0f - 0.15f), background.b * (1.0f - 0.15f), background.a);
}
