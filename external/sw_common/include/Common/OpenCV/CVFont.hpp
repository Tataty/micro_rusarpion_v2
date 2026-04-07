#pragma once
#include <filesystem>
#include <mutex>
#include <opencv2/freetype.hpp>

#include "Common/OpenCV/Common.hpp"

class CVFont {
public:
    static inline const std::string TEXT_FOR_CALCULATE_SIZE = "Qq123";

private:
    class ThreadSafeFreeType {
    private:
        cv::Ptr< cv::freetype::FreeType2 > freeType;
        mutable std::mutex                 mtx;

    public:
        explicit ThreadSafeFreeType(cv::Ptr< cv::freetype::FreeType2 > f) : freeType(f) {}

        cv::Size getTextSize(const std::string& text, int fontHeight, int thickness, int* baseLine) const {
            std::lock_guard< std::mutex > lock(mtx);
            return freeType->getTextSize(text, fontHeight, thickness, baseLine);
        }

        void putText(cv::InputOutputArray img,
                     const std::string&   text,
                     cv::Point            org,
                     int                  fontHeight,
                     cv::Scalar           color,
                     int                  thickness,
                     int                  line_type,
                     bool                 bottomLeftOrigin) const {
            std::lock_guard< std::mutex > lock(mtx);
            freeType->putText(img, text, org, fontHeight, color, thickness, line_type, bottomLeftOrigin);
        }

        bool operator==(const ThreadSafeFreeType& other) const {
            if (this == &other)
                return true;

            std::scoped_lock lock(this->mtx, other.mtx);

            if (!this->freeType && !other.freeType)
                return true;
            if (!this->freeType || !other.freeType)
                return false;

            const int FONT_HEIGHT_FOR_CALCULATE_SIZE = 16;

            cv::Size fontSize =
                    this->freeType->getTextSize(TEXT_FOR_CALCULATE_SIZE, FONT_HEIGHT_FOR_CALCULATE_SIZE, -1, 0);

            int width  = fontSize.width;
            int height = fontSize.height;

            cv::Mat img1 = cv::Mat::zeros(height, width, CV_8UC3);
            cv::Mat img2 = cv::Mat::zeros(height, width, CV_8UC3);

            const cv::Point ORIGIN(0, fontSize.height);
            const int       THICKNESS          = -1;
            const int       LINE_TYPE          = cv::LINE_AA;
            const bool      BOTTOM_LEFT_ORIGIN = true;

            this->freeType->putText(img1,
                                    TEXT_FOR_CALCULATE_SIZE,
                                    ORIGIN,
                                    FONT_HEIGHT_FOR_CALCULATE_SIZE,
                                    WHITE_COLOR,
                                    THICKNESS,
                                    LINE_TYPE,
                                    BOTTOM_LEFT_ORIGIN);
            other.freeType->putText(img2,
                                    TEXT_FOR_CALCULATE_SIZE,
                                    ORIGIN,
                                    FONT_HEIGHT_FOR_CALCULATE_SIZE,
                                    WHITE_COLOR,
                                    THICKNESS,
                                    LINE_TYPE,
                                    BOTTOM_LEFT_ORIGIN);

            return areMatsEqual(img1, img2);
        }

        bool operator!=(const ThreadSafeFreeType& other) const { return !(*this == other); }
    };

    std::shared_ptr< ThreadSafeFreeType > threadSafeFreeType;

    cv::Ptr< cv::freetype::FreeType2 > loadFreeType(const std::filesystem::path& filePath) {
        cv::Ptr< cv::freetype::FreeType2 > freeType = cv::freetype::createFreeType2();
        freeType->loadFontData(filePath, 0);
        return freeType;
    }

public: /* Create */
    CVFont() = default;
    explicit CVFont(const std::filesystem::path& filePath) {
        threadSafeFreeType = std::make_shared< ThreadSafeFreeType >(loadFreeType(filePath));
    }

    CVFont(const CVFont& other) : threadSafeFreeType(other.threadSafeFreeType) {}

    CVFont& operator=(const CVFont& other) {
        if (this != &other) {
            threadSafeFreeType = other.threadSafeFreeType;
        }
        return *this;
    }

    static CVFont load(const std::filesystem::path& filePath) { return CVFont(filePath); }

public: /* Work */
    cv::Size getTextSize(const std::string& text, int fontHeight, int thickness, int* baseLine) const {
        return threadSafeFreeType->getTextSize(text, fontHeight, thickness, baseLine);
    }

    void putText(cv::InputOutputArray img,
                 const std::string&   text,
                 cv::Point            org,
                 int                  fontHeight,
                 cv::Scalar           color,
                 int                  thickness,
                 int                  line_type,
                 bool                 bottomLeftOrigin) const {
        threadSafeFreeType->putText(img, text, org, fontHeight, color, thickness, line_type, bottomLeftOrigin);
    }

public: /* Operator */
    bool operator==(const CVFont& other) const {
        if (this == &other)
            return true;

        return other.threadSafeFreeType == other.threadSafeFreeType;
    }

    bool operator!=(const CVFont& other) const { return !(*this == other); }
};