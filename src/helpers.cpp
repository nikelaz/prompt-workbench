#include "helpers.h"
#include <cmath>

ImVec4 helpers::hex_color(uint32_t hex, float alpha = 1.0f) {
    float r = ((hex >> 16) & 0xFF) / 255.0f;
    float g = ((hex >> 8)  & 0xFF) / 255.0f;
    float b = ((hex >> 0)  & 0xFF) / 255.0f;
    return ImVec4(r, g, b, alpha);
}

float helpers::cosine_similarity(const std::vector<float>& a, const std::vector<float>& b) {
    if (a.empty() || b.empty() || a.size() != b.size()) return 0.0f;
    float dot = 0.0f, norm_a = 0.0f, norm_b = 0.0f;
    for (size_t i = 0; i < a.size(); i++) {
        dot    += a[i] * b[i];
        norm_a += a[i] * a[i];
        norm_b += b[i] * b[i];
    }
    float denom = std::sqrt(norm_a) * std::sqrt(norm_b);
    if (denom == 0.0f) return 0.0f;
    return dot / denom;
}

std::string helpers::truncate_to_lines(const std::string& text, float wrap_width, int max_lines) {
    float line_height = ImGui::GetTextLineHeight();
    float max_height  = line_height * max_lines + ImGui::GetStyle().ItemSpacing.y * (max_lines - 1);

    if (ImGui::CalcTextSize(text.c_str(), nullptr, false, wrap_width).y <= max_height)
        return text;

    int lo = 0, hi = static_cast<int>(text.size());
    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        std::string candidate = text.substr(0, mid) + "...";
        if (ImGui::CalcTextSize(candidate.c_str(), nullptr, false, wrap_width).y <= max_height)
            lo = mid;
        else
            hi = mid - 1;
    }
    return text.substr(0, lo) + "...";
}
