#pragma once

template < typename T > struct Limits {
    T min;
    T max;

    bool contains(const T& value) const { return value >= min && value <= max; }
};