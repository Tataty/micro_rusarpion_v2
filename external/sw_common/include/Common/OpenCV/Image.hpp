#pragma once
#include <filesystem>
#include <opencv2/opencv.hpp>

cv::Mat readImageInMode(const std::filesystem::path& filePath, cv::ImreadModes imreadMode);
cv::Mat readMask(const std::filesystem::path& maskFilePath);

/**
 * @brief Scales an input image while maintaining its aspect ratio and centering it within a target size.
 *
 * This function takes an input image and a desired target size (width and height).
 * It scales the image so that it fits completely within the target size without
 * distortion, meaning one of its dimensions will perfectly match the target,
 * and the other will be less than or equal to the target. The scaled image
 * is then centered within a new canvas of the specified target size.
 *
 * @param input_image The input image (cv::Mat).
 * @param target_size The desired target size (cv::Size) for the output image.
 * @return The scaled and centered image (cv::Mat). Returns an empty cv::Mat if the input_image is empty.
 */
cv::Mat fitImageToSize(const cv::Mat& input_image, const cv::Size& target_size);

cv::Mat zoomImage(const cv::Mat& src, double zoom_factor);

bool isRectInMask(const cv::Rect& rect, const cv::Mat& mask);