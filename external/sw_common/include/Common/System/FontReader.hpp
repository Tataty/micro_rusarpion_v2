#pragma once
#include <filesystem>
#include <opencv2/freetype.hpp>
#include <opencv2/opencv.hpp>

extern std::filesystem::path FONTS_FILE(const std::string& x);

class FontReader {
public:
    static CVFont readFont(const std::string& fileName) { return CVFont::load(FONTS_FILE(fileName)); }
};