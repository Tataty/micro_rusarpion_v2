#pragma once
#include <opencv2/core.hpp>

#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <set>
#include <unordered_map>
#include <vector>

#include "Common/OpenCV/Camera/CameraFormat.hpp"

class CameraInformer {
private:
    template < class T > static int xioctl(int fd, unsigned long req, T* arg) {
        int r;
        do {
            r = ioctl(fd, req, arg);
        } while (r == -1 && errno == EINTR);
        return r;
    }

public:
    struct CameraFormatInfo {
        cv::Size        resolution;
        std::set< int > fpsList;
    };

    typedef std::unordered_map< CameraFormat, std::vector< CameraFormatInfo > > CameraInfo;

    static CameraInfo getInfo(const std::string& device) {

        int fd = open(device.c_str(), O_RDONLY);
        if (fd < 0)
            throw std::runtime_error("Failed to open camera device " + device + ", may be missing");

        CameraInfo cameraInfo;

        for (v4l2_fmtdesc fmtd{};; ++fmtd.index) {
            fmtd.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            if (xioctl(fd, VIDIOC_ENUM_FMT, &fmtd) == -1)
                break;

            for (v4l2_frmsizeenum fsz{};; ++fsz.index) {
                fsz.pixel_format = fmtd.pixelformat;
                if (xioctl(fd, VIDIOC_ENUM_FRAMESIZES, &fsz) == -1)
                    break;

                if (fsz.type != V4L2_FRMSIZE_TYPE_DISCRETE)
                    break;

                cv::Size resolution(fsz.discrete.width, fsz.discrete.height);

                std::set< int > fpsList;

                for (v4l2_frmivalenum fiv{};; ++fiv.index) {
                    fiv.pixel_format = fmtd.pixelformat;
                    fiv.width        = resolution.width;
                    fiv.height       = resolution.height;

                    if (xioctl(fd, VIDIOC_ENUM_FRAMEINTERVALS, &fiv) == -1)
                        break;

                    if (fiv.type != V4L2_FRMIVAL_TYPE_DISCRETE)
                        continue;

                    int fps = fiv.discrete.denominator / fiv.discrete.numerator;

                    if (fps <= 0)
                        continue;

                    fpsList.insert(fps);
                }

                if (fpsList.size() <= 0)
                    break;

                CameraFormat format(fmtd.pixelformat);

                cameraInfo[format].push_back({
                        .resolution = resolution,
                        .fpsList    = std::move(fpsList),
                });
            }
        }

        close(fd);
        return cameraInfo;
    }

    static std::string toString(CameraInfo cameraInfo) {
        std::ostringstream oss;
        for (const auto& [fmt, infos] : cameraInfo) {
            oss << fmt.toString() << ":\n";
            for (const auto& info : infos) {
                oss << "  " << info.resolution.width << "x" << info.resolution.height << " @";
                for (const auto& fps : info.fpsList) {
                    oss << " " << fps;
                }
                oss << " fps\n";
            }
        }
        return oss.str();
    }
};