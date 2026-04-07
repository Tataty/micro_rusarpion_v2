#include "Common/OpenCV/Image.hpp"

cv::Mat fitImageToSize(const cv::Mat& input_image, const cv::Size& target_size) {
    if (input_image.empty()) {
        throw std::runtime_error("Error: Input image is empty.");
    }

    if (input_image.size() == target_size)
        return input_image;

    // Get current dimensions of the input image
    int original_width  = input_image.cols;
    int original_height = input_image.rows;

    // Determine scaling factors for width and height
    double width_scale  = static_cast< double >(target_size.width) / original_width;
    double height_scale = static_cast< double >(target_size.height) / original_height;

    // Use the smaller scale factor to ensure the entire image fits within the target
    double scale = std::min(width_scale, height_scale);

    // Calculate new dimensions of the image after scaling
    int new_width  = static_cast< int >(original_width * scale);
    int new_height = static_cast< int >(original_height * scale);

    // Create a new image for the result with the target dimensions.
    // The type matches the input image to ensure compatibility.
    cv::Mat output_image = cv::Mat::zeros(target_size.height, target_size.width, input_image.type());

    // Resize the input image
    cv::Mat resized_image;
    // cv::INTER_AREA is generally preferred for image shrinking (downsampling)
    cv::resize(input_image, resized_image, cv::Size(new_width, new_height), 0, 0, cv::INTER_AREA);

    // Calculate offsets for centering the resized image within the target canvas
    int offset_x = (target_size.width - new_width) / 2;
    int offset_y = (target_size.height - new_height) / 2;

    // Define the Region Of Interest (ROI) where the resized image will be placed
    cv::Rect roi(offset_x, offset_y, new_width, new_height);

    // Copy the resized image to the center of the new canvas
    // Ensure the ROI dimensions do not exceed the output_image dimensions
    if (roi.x >= 0 && roi.y >= 0 && roi.width <= output_image.cols - roi.x && roi.height <= output_image.rows - roi.y) {
        resized_image.copyTo(output_image(roi));
    } else {
        throw std::runtime_error("Calculated ROI is out of bounds for the output image.");
    }

    return output_image;
}

cv::Mat zoomImage(const cv::Mat& src, double zoom_factor) {
    if (zoom_factor <= 0.0)
        throw std::invalid_argument("Zoom factor must be positive");

    if (zoom_factor == 1.0)
        return src.clone();

    int w = src.cols;
    int h = src.rows;

    int new_w = static_cast< int >(w / zoom_factor);
    int new_h = static_cast< int >(h / zoom_factor);

    int x = (w - new_w) / 2;
    int y = (h - new_h) / 2;

    cv::Rect roi(x, y, new_w, new_h);
    cv::Mat  cropped = src(roi);
    cv::Mat  resized;
    cv::resize(cropped, resized, src.size());

    return resized;
}