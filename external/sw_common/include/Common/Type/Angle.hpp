#pragma once

#include <cmath>
#include <iomanip>
#include <numbers>

class Angle {

private:
    double angleInRadians = 0.0;
    constexpr explicit Angle(double radians)
        : angleInRadians(radians) {}

    static constexpr double radiansToDegrees(double radians) { return radians * (180.0 / std::numbers::pi); }
    static constexpr double degreesToRadians(double degrees) { return degrees * (std::numbers::pi / 180.0); }

public:
    Angle() = default;

    static constexpr Angle fromRadians(double radians) { return Angle(radians); }
    static constexpr Angle fromDegrees(double degrees) { return Angle(degreesToRadians(degrees)); }

    void setRadians(double radians) { this->angleInRadians = radians; }
    void setDegrees(double degrees) { this->angleInRadians = degreesToRadians(degrees); }

    double getRadians() const { return this->angleInRadians; }
    double getDegrees() const { return radiansToDegrees(this->angleInRadians); }

    bool operator<(const Angle& other) const { return angleInRadians < other.angleInRadians; }
    bool operator>(const Angle& other) const { return angleInRadians > other.angleInRadians; }

    bool operator<=(const Angle& other) const { return angleInRadians <= other.angleInRadians; }
    bool operator>=(const Angle& other) const { return angleInRadians >= other.angleInRadians; }

    Angle operator*(double scalar) const { return Angle::fromRadians(angleInRadians * scalar); }
    Angle operator/(double scalar) const { return Angle::fromRadians(angleInRadians / scalar); }

    double operator/(const Angle& other) const { return angleInRadians / other.angleInRadians; }

    Angle operator+(const Angle& other) const { return Angle::fromRadians(angleInRadians + other.angleInRadians); }
    Angle operator-(const Angle& other) const { return Angle::fromRadians(angleInRadians - other.angleInRadians); }

    Angle operator-() const { return Angle::fromRadians(-angleInRadians); }

    Angle& operator*=(double scalar) {
        angleInRadians *= scalar;
        return *this;
    }
    Angle& operator/=(double scalar) {
        angleInRadians /= scalar;
        return *this;
    }

    Angle& operator+=(const Angle& other) {
        angleInRadians += other.angleInRadians;
        return *this;
    }
    Angle& operator-=(const Angle& other) {
        angleInRadians -= other.angleInRadians;
        return *this;
    }

    friend constexpr bool operator==(Angle a, Angle b) noexcept { return a.angleInRadians == b.angleInRadians; }

    Angle getNormalize() const {
        double a = fmod(getDegrees(), 360);
        if (a < 0)
            a += 360;

        return Angle::fromDegrees(a);
    }

    Angle getBetween180() {
        double diff = fmod(getDegrees(), 360);

        if (diff > 180)
            diff -= 360;
        if (diff < -180)
            diff += 360;

        return Angle::fromDegrees(diff);
    }

    static Angle deltaAngles(Angle to_angle, Angle from_angle) {
        double diff = fmod(to_angle.getDegrees() - from_angle.getDegrees(), 360);

        if (diff > 180)
            diff -= 360;
        if (diff < -180)
            diff += 360;

        return Angle::fromDegrees(diff);
    }

public:
    static const Angle Zero;  /* 0° */
    static const Angle Right; /* 90° */
    static const Angle Half;  /* 180° */
    static const Angle Full;  /* 360° */
};

inline constexpr Angle Angle::Zero  = Angle::fromRadians(0.0);
inline constexpr Angle Angle::Right = Angle::fromRadians(std::numbers::pi / 2.0);
inline constexpr Angle Angle::Half  = Angle::fromRadians(std::numbers::pi);
inline constexpr Angle Angle::Full  = Angle::fromRadians(std::numbers::pi * 2.0);

struct Angle2 {
    Angle yaw;
    Angle pitch;

    auto operator<=>(const Angle2&) const = default;

    Angle2 operator+(const Angle2& other) const { return Angle2{ yaw + other.yaw, pitch + other.pitch }; }
    Angle2 operator-(const Angle2& other) const { return Angle2{ yaw - other.yaw, pitch - other.pitch }; }

    Angle2 operator*(double scalar) const { return Angle2{ yaw * scalar, pitch * scalar }; }
    Angle2 operator/(double scalar) const { return Angle2{ yaw / scalar, pitch / scalar }; }

    Angle2& operator+=(const Angle2& other) {
        yaw += other.yaw;
        pitch += other.pitch;
        return *this;
    }

    Angle2& operator-=(const Angle2& other) {
        yaw -= other.yaw;
        pitch -= other.pitch;
        return *this;
    }

    Angle2 operator-() const { return Angle2{ -yaw, -pitch }; }

    bool operator==(const Angle2& other) const = default;

    static Angle2 deltaAngles(Angle2 to_angle, Angle2 from_angle) {
        return Angle2{
            .yaw   = Angle::deltaAngles(to_angle.yaw, from_angle.yaw),
            .pitch = Angle::deltaAngles(to_angle.pitch, from_angle.pitch),
        };
    }
};