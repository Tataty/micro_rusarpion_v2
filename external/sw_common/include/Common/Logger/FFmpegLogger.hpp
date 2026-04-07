#pragma once

#include <memory>
#include "Common/Logger/iLogger.hpp"

extern "C" {
#include <libavutil/log.h>
}

namespace NAMING {
static const std::string FFMPEG = "FFMPEG";
}

class SingletonFFmpegLogger {
private:
    std::shared_ptr< iLogger > logger;

    static SingletonFFmpegLogger* instance;

    SingletonFFmpegLogger(std::shared_ptr< iLogger >& logger) : logger(std::move(logger)) {}

    static void staticCustomFFmpegLog(void*, int level, const char* fmt, va_list vl) {
        if (instance) {
            instance->customLogImpl(level, fmt, vl);
        }
    }

    void customLogImpl(int level, const char* fmt, va_list vl) {
        if (level > av_log_get_level()) {
            return;
        }

        char line[1024];
        vsnprintf(line, sizeof(line), fmt, vl);

        if (level <= AV_LOG_ERROR) {
            logger->error(line);
        } else if (level <= AV_LOG_WARNING) {
            logger->warning(line);
        } else if (level <= AV_LOG_INFO) {
            logger->info(line);
        } else {
            logger->debug(line);
        }
    }

public:
    static void init(std::shared_ptr< iLogger >& logger) {

        instance = new SingletonFFmpegLogger(logger);
        av_log_set_callback(staticCustomFFmpegLog);
        av_log_set_level(AV_LOG_DEBUG);
    }

    SingletonFFmpegLogger(const SingletonFFmpegLogger&)            = delete;
    SingletonFFmpegLogger& operator=(const SingletonFFmpegLogger&) = delete;
};

SingletonFFmpegLogger* SingletonFFmpegLogger::instance = nullptr;