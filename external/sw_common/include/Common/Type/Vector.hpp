#pragma once
#include <cmath>

#include "Common/Type/Angle.hpp"

struct Vec2d {
    double x;
    double y;

    /* MATH */
    double        length() const { return std::sqrt(x * x + y * y); }
    static double length(double x, double y) { return std::sqrt(x * x + y * y); }

    void rotate90() {
        float temp_y = y;
        y            = -x;
        x            = temp_y;
    }

    Vec2d normal() const {
        double len = length();
        if (len > 0) {
            return { x / len, y / len };
        }
        return { 0, 0 };
    }

    void normalize() {
        double len = length();
        if (len > 0) {
            x /= len;
            y /= len;
        }
    }

    Angle        angle() { return Angle::fromRadians(atan2(y, x)); }
    static Angle angle(Vec2d vec) { return Angle::fromRadians(atan2(vec.y, vec.x)); }

    Vec2d clamp() {
        double len = length();

        if (len <= 0)
            return { 0, 0 };
        if (len > 1)
            return { x / len, y / len };

        return { x, y };
    }
    Vec2d clamp(double maxLength) {
        double len = length();

        if (len <= 0)
            return { 0, 0 };
        if (len > maxLength)
            return { x * maxLength / len, y * maxLength / len };

        return { x, y };
    }

    /* OPERATOR */
    Vec2d operator+(const Vec2d& other) const { return { x + other.x, y + other.y }; }
    Vec2d operator-(const Vec2d& other) const { return { x - other.x, y - other.y }; }
    Vec2d operator*(double scalar) const { return { x * scalar, y * scalar }; }
    Vec2d operator/(double scalar) const { return { x / scalar, y / scalar }; }

    Vec2d& operator+=(const Vec2d& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    Vec2d& operator-=(const Vec2d& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vec2d operator-() const { return { -x, -y }; }
    bool  operator==(const Vec2d& other) const { return (x == other.x && y == other.y); }
};
struct Vec3d {
    double x;
    double y;
    double z;

    /* MATH */
    double        length() const { return std::sqrt(x * x + y * y + z * z); }
    static double length(double x, double y, double z) { return std::sqrt(x * x + y * y + z * z); }

    Vec3d normal() const {
        double len = length();
        if (len > 0) {
            return { x / len, y / len, z / len };
        }
        return { 0, 0, 0 };
    }

    void normalize() {
        double len = length();
        if (len > 0) {
            x /= len;
            y /= len;
            z /= len;
        }
    }

    double dot(const Vec3d& other) const { return x * other.x + y * other.y + z * other.z; }

    Vec3d cross(const Vec3d& other) const { return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x }; }

    /* OPERATOR */
    Vec3d operator+(const Vec3d& other) const { return { x + other.x, y + other.y, z + other.z }; }
    Vec3d operator-(const Vec3d& other) const { return { x - other.x, y - other.y, z - other.z }; }
    Vec3d operator*(double scalar) const { return { x * scalar, y * scalar, z * scalar }; }
    Vec3d operator/(double scalar) const { return { x / scalar, y / scalar, z / scalar }; }

    Vec3d& operator+=(const Vec3d& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    Vec3d& operator-=(const Vec3d& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vec3d operator-() const { return { -x, -y, -z }; }
    bool  operator==(const Vec3d& other) const { return (x == other.x && y == other.y && z == other.z); }
};