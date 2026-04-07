/**
 * @file DesktopModule.hpp
 * @author Ruslan L.
 * @brief Testing on display with output and mouse processing
 *
 * @copyright Copyright (c) 2025
**/

#pragma once

#include <opencv2/opencv.hpp>

#include "Common/Module/Callback/MultiCallback.hpp"
#include "Common/Module/iModule.hpp"

/* Module for image output to the screen */
class DesktopModule : public iModule {
public:
    struct Config {
        std::string title;
        bool        isFullScreen;
    };

private:
    const Config config;

public:
    /* Left mouse click position */
    MultiCallback< cv::Point > callbackClick;
    MultiCallback< char >      callbackKey;

    DesktopModule(std::shared_ptr< iModuleLogger >& logger, const Config& config)
        : iModule(logger), config(config), callbackClick(this->logger), callbackKey(this->logger) {

        if (config.isFullScreen) {
            cv::namedWindow(config.title, cv::WINDOW_NORMAL);
            cv::setWindowProperty(config.title, cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
        } else {
            cv::namedWindow(config.title, cv::WINDOW_AUTOSIZE);
        }

        cv::setMouseCallback(config.title, onMouse, this);
    }

    /* Displays the image on the screen */
    void pushFrame(const cv::Mat& image) {
        cv::imshow(config.title, image);

        std::unique_ptr< char > key = std::make_unique< char >(cv::waitKey(1));

        if (*key > 0)
            callbackKey.notify(std::move(key));
    }

private:
    /* Static method for handling mouse events */
    static void onMouse(int event, int x, int y, int flags, void* userdata) {
        DesktopModule* selfDesktopModule = static_cast< DesktopModule* >(userdata);

        if (event == cv::EVENT_LBUTTONDOWN)
            selfDesktopModule->callbackClick.notify(std::make_unique< cv::Point >(cv::Point(x, y)));
    }
};
