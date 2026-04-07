#pragma once

#include <cmath>
#include <iomanip>
#include <numbers>

class Angle {
private:
    double angleInRadians = 0.f;
    constexpr explicit Angle(double radians) : angleInRadians(radians) {}

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

    static Angle deltaAngles(Angle to_angle, Angle from_angle) {
        double diff = fmod(to_angle.getDegrees() - from_angle.getDegrees(), 360);

        if (diff > 180)
            diff -= 360;
        if (diff < -180)
            diff += 360;

        return Angle::fromDegrees(diff);
    }
};

struct Angle2 {
    Angle yaw;
    Angle pitch;

    auto operator<=>(const Angle2&) const = default;
};