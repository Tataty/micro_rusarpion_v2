#pragma once

#include <dirent.h>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

enum ScanType { ADD, REMOVE };

static inline ScanType strToScanType(std::string str) {

    if (str == "add") {
        return ScanType::ADD;
    } else if (str == "remove") {
        return ScanType::REMOVE;
    }

    throw std::runtime_error("ScanType not supported: " + str);
};

static inline std::string toString(ScanType scanType) {
    switch (scanType) {
    case ScanType::ADD:
        return "add";
    case ScanType::REMOVE:
        return "remove";
    default:
        break;
    }
    throw std::runtime_error("ScanType not supported: " + static_cast< int >(scanType));
};

// TODO: refactoring to simple iDeviceScanner
class iCameraScanner {
public:
    static inline std::string CAMERA_DEVICE_PREFIX = "camera-";

protected:
    std::mutex mutex;

public:
    virtual ~iCameraScanner() = default;

    struct ScanInfo {
        ScanType    type;
        std::string device;
    };

    virtual std::optional< ScanInfo > scan() = 0;

    virtual void start() {}
    virtual void stop() {};

    static inline bool isCameraDevice(std::string device) {

        size_t last_slash_pos = device.rfind('/');

        if (last_slash_pos == device.length() - 1) {
            return false;
        }

        if (last_slash_pos == std::string::npos) {
            return device.find(CAMERA_DEVICE_PREFIX) == 0;
        }

        std::string substring_after_slash = device.substr(last_slash_pos + 1);

        return substring_after_slash.find(CAMERA_DEVICE_PREFIX) == 0;
    }

    static std::vector< std::string > getAvailableCameraDevices() {
        std::vector< std::string > cameraDevices;
        DIR*                       dir;
        struct dirent*             entry;

        dir = opendir("/dev");
        if (dir == NULL)
            return cameraDevices;

        while ((entry = readdir(dir)) != NULL) {
            if (isCameraDevice(entry->d_name)) {
                std::string devicePath = "/dev/";
                devicePath += entry->d_name;
                cameraDevices.push_back(devicePath);
            }
        }

        closedir(dir);
        return cameraDevices;
    }
};