#pragma once

#include <condition_variable>

#include "Common/OpenCV/Common.hpp"
#include "Common/OpenCV/VideoStream/MatPipeWriter.hpp"
#include "Common/OpenCV/VideoStream/iVideoStreamer.hpp"

class PipeVideoStreamer : public iVideoStreamer {
public:
    struct Config {
        iVideoStreamer::Config streamer;
        std::string            pipeline;
    };

private:
    std::mutex   mutex;
    const Config config;

    MatPipeWriter matPipeWriter;
    cv::Mat       yuvFrame;

    std::condition_variable readyCondition;

public:
    PipeVideoStreamer(const std::shared_ptr< iModuleLogger >& logger, const Config& config)
        : iVideoStreamer(logger), config(config) {}
    ~PipeVideoStreamer() { kill(); }

    void pushFrame(const cv::Mat& frame) override {

        try {

            std::lock_guard< std::mutex > lock(mutex);

            if (matPipeWriter.isReady())
                matPipeWriter.writeFrame(frame);

        } catch (const std::exception& e) {

            logger->error(e.what());
            stop();
        }
    }

protected:
    void init() override {
        std::lock_guard< std::mutex > lock(mutex);

        this->logger->info("Init FFmpeg pipeline: {}", config.pipeline);
        matPipeWriter.start(config.pipeline);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::chrono::duration< double > action() override {
        std::unique_lock< std::mutex > lock(mutex);

        readyCondition.wait(lock, [this]() {
            return !matPipeWriter.isReady() || !isWork();
        });

        if (!isWork())
            return 0ms;

        throw std::runtime_error("Ready condition is failed");
    }

    void stop() override {
        std::lock_guard< std::mutex > lock(mutex);

        matPipeWriter.stop();
        readyCondition.notify_one();
    }
};