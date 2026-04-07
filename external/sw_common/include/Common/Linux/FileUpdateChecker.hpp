#pragma once

#include <filesystem>

class FileUpdateChecker {
private:
    std::filesystem::path           filePath;
    std::filesystem::file_time_type lastWriteTime;

public:
    FileUpdateChecker(std::filesystem::path filePath) : filePath(filePath) {}

    bool isFileUpdate() {
        auto currentWriteTime = std::filesystem::last_write_time(filePath);

        if (currentWriteTime != lastWriteTime) {
            lastWriteTime = currentWriteTime;
            return true;
        }
        return false;
    }

    void setAsUpdate() { lastWriteTime = std::filesystem::last_write_time(filePath); }
};