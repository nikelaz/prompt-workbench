#include "fonts.h"
#include <filesystem>

using namespace std::filesystem;

fonts::Fonts fonts::load(ImGuiIO *io)
{
    io->Fonts->Clear(); 

    path exePath = current_path();
    path fontPath = exePath / "fonts" / "Inter-Regular.ttf";
    std::string font_path_str = fontPath.string();

    Fonts fonts = {};

    fonts.regular_medium = io->Fonts->AddFontFromFileTTF(
        font_path_str.c_str(),
        16.0f
    );

    fonts.regular_small = io->Fonts->AddFontFromFileTTF(
        font_path_str.c_str(),
        13.0f
    );

    io->FontDefault = fonts.regular_medium;

    return fonts;
}
