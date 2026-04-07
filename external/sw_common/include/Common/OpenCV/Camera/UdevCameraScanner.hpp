#pragma once

#include <libudev.h>
#include <unistd.h>
#include <atomic>
#include <optional>
#include <sstream>

#include "Common/Linux/Common.hpp"
#include "Common/OpenCV/Camera/iCameraScanner.hpp"

class UdevCameraScanner : public iCameraScanner {
public:
    static constexpr char DUMMY_SYMBOL = 'x';

private:
    std::atomic< bool > stop_flag{ true };
    int                 shutdown_pipe_fds[2];

    struct udev*         udev;
    struct udev_monitor* mon;

    int udev_fd;
    int pipe_fd;

    /**
     * @brief Extracts the path from the devlinks string starting with "/dev/camera-".
     *
     * @param devlinks A string containing one or more device paths,
     * separated by spaces. For example:
     * "/dev/v4l/by-path/pci-foo /dev/camera-1-1.2 /dev/v4l/by-id/usb-bar"
     * @return std::optional<std::string> containing the found path,
     * or std::nullopt if nothing is found or the input string is empty.
    **/
    std::optional< std::string > extractCameraDevLink(const char* devlinks) {
        if (!devlinks) {
            return std::nullopt;
        }

        std::istringstream iss(devlinks);
        std::string        currentPath;

        while (iss >> currentPath) {
            if (isCameraDevice(currentPath)) {
                return currentPath;
            }
        }

        return std::nullopt;
    }

public:
    UdevCameraScanner() : shutdown_pipe_fds{ -1, -1 }, udev(nullptr), mon(nullptr), udev_fd(-1), pipe_fd(-1) {}

    ~UdevCameraScanner() { stop(); }

    std::optional< ScanInfo > scan() override {
        if (stop_flag.load())
            return std::nullopt;

        std::lock_guard< std::mutex > lock(mutex);

        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(udev_fd, &fds);
        FD_SET(pipe_fd, &fds);

        int max_fd = std::max(udev_fd, pipe_fd);

        /* Wait signal */
        int ret = select(max_fd + 1, &fds, NULL, NULL, NULL);

        if (ret < 0) {
            if (errno == EINTR)
                return std::nullopt;
            throw std::runtime_error("Scanner failed to select udev signal");
        }

        /* Stop signal */
        if (FD_ISSET(pipe_fd, &fds))
            return std::nullopt;

        /* Udev signal */
        if (!FD_ISSET(udev_fd, &fds))
            return std::nullopt;

        struct udev_device* dev = udev_monitor_receive_device(mon);
        if (!dev)
            return std::nullopt;

        std::optional< ScanInfo > scanInfo = std::nullopt;
        const char*               action   = udev_device_get_action(dev);

        if (action) {
            try {
                ScanType actionType = strToScanType(std::string(action));

                const char*                  devlinks      = udev_device_get_property_value(dev, "DEVLINKS");
                std::optional< std::string > cameraDevLink = extractCameraDevLink(devlinks);

                if (cameraDevLink.has_value()) {
                    scanInfo = ScanInfo{
                        .type   = actionType,
                        .device = cameraDevLink.value(),
                    };
                }
            } catch (const std::exception& e) {
            }
        }

        udev_device_unref(dev);
        return scanInfo;
    }

    void start() override {
        if (!stop_flag.load()) {
            return;
        }

        try {
            std::lock_guard< std::mutex > lock(mutex);

            if (pipe(shutdown_pipe_fds) == -1) {
                throw std::runtime_error("Failed to create pipe");
            }

            udev = udev_new();
            if (!udev) {
                throw std::runtime_error("Failed to create udev context");
            }

            mon = udev_monitor_new_from_netlink(udev, "udev");
            if (!mon) {
                throw std::runtime_error("Failed to create udev monitor");
            }

            udev_monitor_filter_add_match_subsystem_devtype(mon, "video4linux", NULL);
            udev_monitor_enable_receiving(mon);

            udev_fd = udev_monitor_get_fd(mon);
            pipe_fd = shutdown_pipe_fds[0];

            stop_flag.store(false);

        } catch (...) {
            stop();
            throw;
        }
    }

    void stop() override {
        if (stop_flag.load()) {
            return;
        }

        stop_flag.store(true);

        {

            /* Wake up select() in scan() */
            if (shutdown_pipe_fds[1] != -1) {
                IGNORE_RESULT(write(shutdown_pipe_fds[1], &DUMMY_SYMBOL, 1));
            }

            std::lock_guard< std::mutex > lock(mutex);

            /* Clean up udev resources */
            if (mon) {
                udev_monitor_unref(mon);
                mon = nullptr;
            }
            if (udev) {
                udev_unref(udev);
                udev = nullptr;
            }

            /* Clean up pipe file descriptors */
            if (shutdown_pipe_fds[0] != -1) {
                close(shutdown_pipe_fds[0]);
                shutdown_pipe_fds[0] = -1;
                pipe_fd              = -1;
            }
            if (shutdown_pipe_fds[1] != -1) {
                close(shutdown_pipe_fds[1]);
                shutdown_pipe_fds[1] = -1;
            }
            udev_fd = -1;
        }
    }
};