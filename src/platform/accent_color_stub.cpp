#include "accent_color.h"
#include "helpers.h"

ImVec4 platform::get_accent_color() {
    return helpers::hex_color(0x303F9F, 1.0f);
}
