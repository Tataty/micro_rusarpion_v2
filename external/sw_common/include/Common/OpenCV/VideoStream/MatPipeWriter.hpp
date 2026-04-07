#pragma once
#include <opencv2/opencv.hpp>

#include "Common/Linux/PipeWriter.hpp"

class MatPipeWriter : public PipeWriter {
private:
    cv::Mat yuvFrame;

public:
    void writeFrame(const cv::Mat& bgrFrame) {
        if (bgrFrame.empty() || bgrFrame.type() != CV_8UC3) {
            throw std::runtime_error("Invalid frame format! Expected CV_8UC3 (BGR)");
        }

        cv::cvtColor(bgrFrame, yuvFrame, cv::COLOR_BGR2YUV_I420);
        if (!yuvFrame.isContinuous())
            yuvFrame = yuvFrame.clone();

        const size_t yuvFrameSize = yuvFrame.total() * yuvFrame.elemSize();

        writeData(yuvFrame.data, yuvFrameSize);
    }
};