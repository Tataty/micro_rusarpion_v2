#pragma once

#include <opencv2/opencv.hpp>

class CPUImage {
private:
    cv::Mat mat;

public:
    CPUImage(cv::Mat& cvMat) : mat(cvMat) {}
    /* other constructor from CopyMat */

    cv::Size getResolution() { return mat.size(); }
};

class DMAImage {
private:
    int      fd;
    char*    virtualAddress;
    cv::Size resolution;
    // _Rga_SURF_FORMAT format;

public:
    explicit DMAImage(const CPUImage& cpuImage) {
        //... imcopy
    }
    cv::Size getResolution() { return resolution; }
};

void imageCopy(const CPUImage& cpuImage, DMAImage& dmaImage) {
    // ...
}

void imageCopy(const DMAImage& dmaImage, CPUImage& cpuImage) {
    // ...
}

void drawRectangle(const DMAImage& dmaImage) {
    // ... imrectangle
}

void imageMix(const DMAImage& inputDMAImage, DMAImage& outputDMAImage) {
    // ... improcess
}
