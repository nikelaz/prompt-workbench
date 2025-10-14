#include "helpers.h"

ImVec4 helpers::hexColor(uint32_t hex, float alpha = 1.0f) {
    float r = ((hex >> 16) & 0xFF) / 255.0f;
    float g = ((hex >> 8)  & 0xFF) / 255.0f;
    float b = ((hex >> 0)  & 0xFF) / 255.0f;
    return ImVec4(r, g, b, alpha);
}
