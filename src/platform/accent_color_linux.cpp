#include "accent_color.h"
#include "helpers.h"
#include <cstdio>
#include <string>
#include <fstream>
#include <algorithm>

static ImVec4 gnome_named_color(const std::string& name) {
    if (name == "blue")   return helpers::hex_color(0x3584e4, 1.0f);
    if (name == "teal")   return helpers::hex_color(0x2190a4, 1.0f);
    if (name == "green")  return helpers::hex_color(0x3a944a, 1.0f);
    if (name == "yellow") return helpers::hex_color(0xc88800, 1.0f);
    if (name == "orange") return helpers::hex_color(0xe66100, 1.0f);
    if (name == "red")    return helpers::hex_color(0xe01b24, 1.0f);
    if (name == "pink")   return helpers::hex_color(0xd56199, 1.0f);
    if (name == "purple") return helpers::hex_color(0x9141ac, 1.0f);
    if (name == "slate")  return helpers::hex_color(0x6f8396, 1.0f);
    return ImVec4(-1, 0, 0, 0); // not found
}

static ImVec4 try_gnome() {
    FILE* pipe = popen("gsettings get org.gnome.desktop.interface accent-color 2>/dev/null", "r");
    if (!pipe) return ImVec4(-1, 0, 0, 0);

    char buf[64] = {};
    fgets(buf, sizeof(buf), pipe);
    pclose(pipe);

    std::string raw(buf);
    // Strip whitespace, quotes, newlines
    raw.erase(std::remove_if(raw.begin(), raw.end(), [](char c) {
        return c == '\'' || c == '"' || c == '\n' || c == '\r' || c == ' ';
    }), raw.end());

    if (raw.empty()) return ImVec4(-1, 0, 0, 0);
    return gnome_named_color(raw);
}

static ImVec4 try_kde() {
    const char* home = getenv("HOME");
    if (!home) return ImVec4(-1, 0, 0, 0);

    std::ifstream file(std::string(home) + "/.config/kdeglobals");
    if (!file.is_open()) return ImVec4(-1, 0, 0, 0);

    std::string line;
    while (std::getline(file, line)) {
        if (line.rfind("AccentColor=", 0) != 0) continue;
        std::string val = line.substr(12);
        int r, g, b;
        if (sscanf(val.c_str(), "%d,%d,%d", &r, &g, &b) == 3) {
            return ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
        }
    }
    return ImVec4(-1, 0, 0, 0);
}

ImVec4 platform::get_accent_color() {
    ImVec4 color = try_gnome();
    if (color.x >= 0) return color;

    color = try_kde();
    if (color.x >= 0) return color;

    return helpers::hex_color(0x303F9F, 1.0f);
}
