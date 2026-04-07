#pragma once

#include <math.h>

class Percent {

private:
    double valueNormalize = 0.f;
    explicit constexpr Percent(double valueNormalize) : valueNormalize(valueNormalize) {}

    static constexpr double hundredToNormalize(int hundred) { return hundred / 100.; }
    static int              normalizeToHundred(double normalize) { return static_cast< int >(normalize * 100.0); }

public:
    Percent() = default;

    static constexpr Percent fromHundred(int hundred) { return Percent(hundredToNormalize(hundred)); }
    static constexpr Percent fromNormalize(double normalize) { return Percent(normalize); }
    static Percent           fromRangeValue(int value, int min, int max) {
        return Percent(static_cast< double >(value - min) / (max - min));
    }
    static Percent fromRangeValue(double value, double min, double max) { return Percent((value - min) / (max - min)); }

    constexpr bool operator<(const Percent& other) const { return valueNormalize < other.valueNormalize; }
    constexpr bool operator>(const Percent& other) const { return valueNormalize > other.valueNormalize; }

    constexpr Percent operator*(double scalar) const { return Percent::fromNormalize(valueNormalize * scalar); }
    constexpr Percent operator/(double scalar) const { return Percent::fromNormalize(valueNormalize / scalar); }

    double operator/(const Percent& other) const { return valueNormalize / other.valueNormalize; }

    constexpr Percent operator+(const Percent& other) const {
        return Percent::fromNormalize(valueNormalize + other.valueNormalize);
    }
    constexpr Percent operator-(const Percent& other) const {
        return Percent::fromNormalize(valueNormalize - other.valueNormalize);
    }

    Percent operator-() const { return Percent::fromNormalize(-valueNormalize); }

    Percent& operator*=(double scalar) {
        valueNormalize *= scalar;
        return *this;
    }
    Percent& operator/=(double scalar) {
        valueNormalize /= scalar;
        return *this;
    }

    Percent& operator+=(const Percent& other) {
        valueNormalize += other.valueNormalize;
        return *this;
    }
    Percent& operator-=(const Percent& other) {
        valueNormalize -= other.valueNormalize;
        return *this;
    }

    void setNormalize(double normilize) { this->valueNormalize = normilize; }
    void setHundred(int hundred) { this->valueNormalize = hundredToNormalize(hundred); }

    constexpr double getNormalize() const { return valueNormalize; }
    int              getHundred() const { return normalizeToHundred(valueNormalize); }

    int getValueByRange(int min, int max) const {
        return static_cast< int >(round((max - min) * valueNormalize + min));
    }
    double getValueByRange(double min, double max) const { return (max - min) * valueNormalize + min; }
};

static constexpr Percent PERCENT_QUARTER = Percent::fromHundred(25);
static constexpr Percent PERCENT_HALF    = Percent::fromHundred(50);
static constexpr Percent PERCENT_HUNDRED = Percent::fromHundred(100);