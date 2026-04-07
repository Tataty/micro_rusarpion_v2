#pragma once

#include <algorithm>
#include <vector>

#include "Common/Type/Angle.hpp"

class BallisticTable {
public:
    struct Tables {
        std::vector< double > distances;
        std::vector< double > anglesInDegrees;
        std::vector< double > times;

        auto operator<=>(const Tables&) const = default;
    };

private:
    std::shared_ptr< Tables > tables;

    /* Linear interpolation of values */
    double interpolateByDistance(const std::vector< double >& table, double distance) const {
        if (tables->distances.empty())
            throw std::runtime_error("Ballistic table empty");

        if (distance <= tables->distances.front())
            return table.front();
        if (distance >= tables->distances.back())
            return table.back();

        for (size_t i = 0; i < tables->distances.size() - 1; ++i) {
            if (distance >= tables->distances[i] && distance <= tables->distances[i + 1]) {
                double t = (distance - tables->distances[i]) / (tables->distances[i + 1] - tables->distances[i]);
                return table[i] + t * (table[i + 1] - table[i]);
            }
        }

        return table.back();
    }

    void checkTables() {
        if (tables->distances.empty() || tables->anglesInDegrees.empty() || tables->times.empty()) {
            throw std::invalid_argument("All data vectors must be non-empty");
        }

        if (tables->distances.size() != tables->anglesInDegrees.size() ||
            tables->distances.size() != tables->times.size()) {
            throw std::invalid_argument("All data vectors must be same size.");
        }
    }

    void sortTables() {
        std::sort(tables->distances.begin(), tables->distances.end());
        std::sort(tables->anglesInDegrees.begin(), tables->anglesInDegrees.end());
        std::sort(tables->times.begin(), tables->times.end());
    }

public:
    BallisticTable() = default;

    /* Move */
    BallisticTable(BallisticTable&& other) : tables(std::move(other.tables)) {}
    BallisticTable& operator=(BallisticTable&& other) {
        if (&other == this)
            return *this;

        tables = std::move(other.tables);
        return *this;
    }

    /* Shared */
    BallisticTable(const BallisticTable& other) : tables(other.tables) {}

    BallisticTable& operator=(const BallisticTable& other) {
        if (&other == this)
            return *this;

        tables = other.tables;
        return *this;
    }

    /* Copy */
    BallisticTable(const Tables& tables) { this->tables = std::make_shared< Tables >(tables); }

    BallisticTable& operator=(const Tables& tables) {
        this->tables = std::make_shared< Tables >(tables);
        return *this;
    }

    /* Get pitch for a given distance */
    Angle getPitch(double distance) const {
        return Angle::fromDegrees(interpolateByDistance(tables->anglesInDegrees, distance));
    }
    /* Get flight time for a given distance in seconds */
    double getFlightTime(double distance) const { return interpolateByDistance(tables->times, distance); }

    friend bool operator==(const BallisticTable& a, const BallisticTable& b) noexcept {
        return a.tables.get() == b.tables.get();
    }
};