#pragma once
#include <opencv2/freetype.hpp>

#include "Common/OpenCV/CVFont.hpp"

#include "Common/Test/TestFolders.hpp"

class TestFont {
public:
    static cv::Ptr< cv::freetype::FreeType2 > readFont(const std::string& fileName) {
        cv::Ptr< cv::freetype::FreeType2 > freeType;

        freeType = cv::freetype::createFreeType2();
        freeType->loadFontData(TEST_FILE(fileName), 0);

        return freeType;
    }

    static CVFont readCVFont(const std::string& fileName) { return CVFont::load(TEST_FILE(fileName)); }
};