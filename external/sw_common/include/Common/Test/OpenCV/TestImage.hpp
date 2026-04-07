#pragma once

#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>

#include "Common/OpenCV/GUI/Common.hpp"
#include "Common/Test/TestFolders.hpp"

class TestImage {
public:
    static inline const cv::Scalar WHITE_COLOR        = cv::Scalar(255, 255, 255);
    static inline const cv::Scalar BLACK_COLOR        = cv::Scalar(0, 0, 0);
    static inline const cv::Size   DEFAULT_IMAGE_SIZE = cv::Size(640, 480);

    static cv::Mat readImage(const std::string& fileName) { return cv::imread(TEST_FILE(fileName), cv::IMREAD_COLOR); }

    static cv::Mat readImageToSize(const std::string& fileName, const cv::Size& toSize) {
        cv::Mat resizeImage;
        cv::resize(readImage(fileName), resizeImage, toSize);
        return resizeImage;
    }

    static cv::Mat whiteImage(cv::Size size = DEFAULT_IMAGE_SIZE) {
        return cv::Mat(size, CV_8UC3, cv::Scalar(255, 255, 255));
    }
    static cv::Mat blackImage(cv::Size size = DEFAULT_IMAGE_SIZE) { return cv::Mat::zeros(size, CV_8UC3); }

    static void drawCenterLines(cv::Mat& image, cv::Scalar color) {
        cv::line(image, leftCenter(image), rightCenter(image), color);
        cv::line(image, topCenter(image), bottomCenter(image), color);
    }

    static void drawBorder(cv::Mat& image, cv::Scalar color) {
        cv::rectangle(image, cv::Rect(0, 0, image.cols, image.rows), color, 1, cv::LINE_8);
    }
};

/*--------------------------------------------------------*/
#define ASSERT_MAT_EQ(expected, actual)                    \
    do {                                                   \
        ASSERT_TRUE((expected).size() == (actual).size()); \
        ASSERT_TRUE((expected).type() == (actual).type()); \
                                                           \
        cv::Mat diff;                                      \
        cv::compare(expected, actual, diff, cv::CMP_NE);   \
        int nonzero = cv::countNonZero(diff.reshape(1));   \
                                                           \
        ASSERT_TRUE(nonzero == 0);                         \
    } while (0)

#define ASSERT_PNG_EQ(expected, filepng)                   \
    do {                                                   \
        cv::Mat actual = TestImage::readImage(filepng);    \
        if (actual.empty())                                \
            cv::imwrite(filepng, expected);                \
                                                           \
        ASSERT_TRUE((expected).size() == (actual).size()); \
        ASSERT_TRUE((expected).type() == (actual).type()); \
                                                           \
        cv::Mat diff;                                      \
        cv::compare(expected, actual, diff, cv::CMP_NE);   \
        int nonzero = cv::countNonZero(diff.reshape(1));   \
                                                           \
        if (nonzero != 0)                                  \
            cv::imwrite(filepng, expected);                \
        ASSERT_TRUE(nonzero == 0);                         \
    } while (0)
