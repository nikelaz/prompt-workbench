#include "fonts.h"
#include "imgui_freetype.h"
#include <filesystem>

using namespace std::filesystem;

static void merge_emoji(ImGuiIO *io, path exePath, float size)
{
    path emojiPath = exePath / "fonts" / "TwemojiMozilla.ttf";
    std::string emoji_path_str = emojiPath.string();

    static ImWchar emoji_ranges[] = { 0x1, 0x1FFFF, 0 };
    ImFontConfig emoji_cfg;
    emoji_cfg.MergeMode = true;
    emoji_cfg.OversampleH = 1;
    emoji_cfg.OversampleV = 1;
    emoji_cfg.PixelSnapH = true;
    emoji_cfg.FontLoaderFlags = ImGuiFreeTypeLoaderFlags_LoadColor;
    io->Fonts->AddFontFromFileTTF(
        emoji_path_str.c_str(),
        size,
        &emoji_cfg,
        emoji_ranges
    );
}

fonts::Fonts fonts::load(ImGuiIO *io, float dpi_scale)
{
    io->Fonts->Clear();
    io->Fonts->FontLoaderFlags = ImGuiFreeTypeLoaderFlags_NoHinting;

    path exePath = current_path();
    path fontPath = exePath / "fonts" / "Inter-Regular.ttf";
    std::string font_path_str = fontPath.string();

    Fonts fonts = {};

    fonts.regular_medium = io->Fonts->AddFontFromFileTTF(
        font_path_str.c_str(),
        16.0f * dpi_scale
    );
    merge_emoji(io, exePath, 16.0f * dpi_scale);

    fonts.regular_small = io->Fonts->AddFontFromFileTTF(
        font_path_str.c_str(),
        13.0f * dpi_scale
    );
    merge_emoji(io, exePath, 13.0f * dpi_scale);

    fonts.heading = io->Fonts->AddFontFromFileTTF(
        font_path_str.c_str(),
        24.0f * dpi_scale
    );
    merge_emoji(io, exePath, 24.0f * dpi_scale);

    io->FontDefault = fonts.regular_medium;

    return fonts;
}
