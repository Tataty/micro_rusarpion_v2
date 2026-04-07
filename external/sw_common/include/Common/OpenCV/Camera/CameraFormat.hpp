#pragma once

#include <string>

struct CameraFormat {
private:
    uint32_t raw{};

public:
    static const CameraFormat MJPG;
    static const CameraFormat YUYV;

    static constexpr uint32_t MJPG_RAW = 0x47504A4D;
    static constexpr uint32_t YUYV_RAW = 0x56595559;

    constexpr CameraFormat() noexcept : raw(0) {}

    constexpr explicit CameraFormat(uint32_t v) : raw(v) {}

    constexpr explicit CameraFormat(const char (&s)[5])
        : raw((uint32_t)(uint8_t)s[0] | ((uint32_t)(uint8_t)s[1] << 8) | ((uint32_t)(uint8_t)s[2] << 16) |
              ((uint32_t)(uint8_t)s[3] << 24)) {}

    explicit CameraFormat(const std::string& s)
        : raw((uint32_t)(uint8_t)s[0] | ((uint32_t)(uint8_t)s[1] << 8) | ((uint32_t)(uint8_t)s[2] << 16) |
              ((uint32_t)(uint8_t)s[3] << 24)) {}

    constexpr uint32_t value() const noexcept { return raw; }

    std::string toString() const {
        char s[5]{ char(raw & 0xFF), char((raw >> 8) & 0xFF), char((raw >> 16) & 0xFF), char((raw >> 24) & 0xFF), 0 };
        return std::string(s);
    }

    friend constexpr bool operator==(CameraFormat a, CameraFormat b) noexcept { return a.raw == b.raw; }
};

constexpr CameraFormat CameraFormat::MJPG("MJPG");
constexpr CameraFormat CameraFormat::YUYV("YUYV");

template <> struct std::hash< CameraFormat > {
    size_t operator()(const CameraFormat& f) const noexcept { return std::hash< uint32_t >{}(f.value()); }
};