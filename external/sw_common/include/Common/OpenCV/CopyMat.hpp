#pragma once
#include <opencv2/opencv.hpp>

struct CopyMat {
    cv::Mat mat;

    CopyMat()  = default;
    ~CopyMat() = default;

    /* Copy */
    CopyMat(const CopyMat& other) { mat = other.mat.clone(); }

    CopyMat& operator=(const CopyMat& other) {
        if (this != &other) {
            mat = other.mat.clone();
        }
        return *this;
    }

    /* Move */
    CopyMat(CopyMat&& x) : mat(x.mat) {}

    CopyMat& operator=(CopyMat&& x) {
        if (&x == this)
            return *this;

        mat = x.mat;
        return *this;
    }

    /* cv::Mat */
    CopyMat(const cv::Mat& mat) : mat(mat) {}

    CopyMat& operator=(const cv::Mat& otherMat) {
        mat = otherMat;
        return *this;
    }

    cv::Mat& operator*() { return mat; }
};