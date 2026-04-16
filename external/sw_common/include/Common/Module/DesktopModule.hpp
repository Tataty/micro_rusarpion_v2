/**
 * @file DesktopModule.hpp
 * @author Ruslan L.
 * @brief Testing on display with output and mouse processing
 *
 * @copyright Copyright (c) 2025
**/

#pragma once

#include "Common/OpenCV/Common.hpp"

#include "Common/Module/Callback/MultiCallback.hpp"
#include "Common/Module/iModule.hpp"

/* Module for image output to the screen */
class DesktopModule : public iModule {
public:
    struct Config {
        std::string title;
        bool        isFullScreen = false;
    };

private:
    const Config config;

    std::optional< cv::Size > lastImageSize;

public:
    /* Left mouse click position */
    MultiCallback< cv::Point2d > normClickCallback;
    MultiCallback< cv::Point >   clickCallback;
    MultiCallback< char >        keyCallback;

    DesktopModule(const std::shared_ptr< iModuleLogger >& logger, const Config& config)
        : iModule(logger), config(config), normClickCallback(logger), clickCallback(logger), keyCallback(logger) {

        if (config.isFullScreen) {
            cv::namedWindow(config.title, cv::WINDOW_NORMAL);
            cv::setWindowProperty(config.title, cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
        } else {
            cv::namedWindow(config.title, cv::WINDOW_AUTOSIZE);
        }

        cv::setMouseCallback(config.title, onMouse, this);
    }

    /* Displays the image on the screen */
    void pushImage(const cv::Mat& image) {
        cv::imshow(config.title, image);

        lastImageSize = imageSize(image);

        std::unique_ptr< char > key = std::make_unique< char >(cv::waitKey(1));

        if (*key > 0)
            keyCallback.notify(std::move(key));
    }

private:
    /* Static method for handling mouse events */
    static void onMouse(int event, int x, int y, int flags, void* userdata) {
        DesktopModule* selfDesktopModule = static_cast< DesktopModule* >(userdata);

        if (event == cv::EVENT_LBUTTONDOWN) {
            auto clickPoint = std::make_unique< cv::Point >(x, y);

            if (selfDesktopModule->lastImageSize.has_value()) {
                auto normClickPoint = std::make_unique< cv::Point2d >();
                normClickPoint->x =
                        static_cast< double >(clickPoint->x) / selfDesktopModule->lastImageSize.value().width;
                normClickPoint->y =
                        static_cast< double >(clickPoint->y) / selfDesktopModule->lastImageSize.value().height;

                selfDesktopModule->normClickCallback.notify(std::move(normClickPoint));
            }

            selfDesktopModule->clickCallback.notify(std::move(clickPoint));
        }
    }
};
