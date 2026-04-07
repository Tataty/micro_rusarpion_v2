#pragma once
#include <opencv2/core.hpp>
#include <opencv2/core/utils/logger.hpp>

#include "Common/Logger/iLogger.hpp"

namespace NAMING {
static const std::string OPENCV = "OPENCV";
}

class OpenCVLogger {
protected:
    std::shared_ptr< iLogger > logger;

    static int
    errorHandler(int, const char* func_name, const char* err_msg, const char* file_name, int line, void* userdata) {

        OpenCVLogger* self = static_cast< OpenCVLogger* >(userdata);

        if (self && self->logger) {
            self->logger->error("{} in function {} at {}:{}", err_msg, func_name, file_name, line);
        }

        return 0;
    }

public:
    /* The OpenCVLogger object must exist at all times! */
    OpenCVLogger(std::shared_ptr< iLogger >& logger) : logger(std::move(logger)) {
        cv::redirectError(errorHandler, this);
        cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);
    }
};