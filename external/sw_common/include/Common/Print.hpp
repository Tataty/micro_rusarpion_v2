#pragma once

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace Print {

/* INTEGER */
struct IntegerPrinter {
    int value;
};

inline std::ostream& operator<<(std::ostream& os, IntegerPrinter bp) { return os << bp.value; }

inline IntegerPrinter asInteger(int v) { return IntegerPrinter{ v }; }

/* BOOL */
struct BoolPrinter {
    bool value;
};

inline std::ostream& operator<<(std::ostream& os, BoolPrinter bp) { return os << std::boolalpha << bp.value; }

template < typename T > BoolPrinter asBool(T v) { return BoolPrinter{ static_cast< bool >(v) }; }

/* PERCENT */
struct PercentPrinter {
    int value;
};

inline std::ostream& operator<<(std::ostream& os, PercentPrinter bp) { return os << bp.value << '%'; }

inline PercentPrinter asPercent(int v) { return PercentPrinter{ v }; }

/* ANGLE */
struct AnglePrinter {
    double value;
};

inline std::ostream& operator<<(std::ostream& os, AnglePrinter ap) { return os << ap.value << "°"; }

inline AnglePrinter asDegrees(double v) { return AnglePrinter{ v }; }

/* HEX */
struct HexPrinter {
    uint8_t value;
};

inline std::ostream& operator<<(std::ostream& os, HexPrinter hp) {
    std::ios::fmtflags f       = os.flags();
    char               oldfill = os.fill();

    os << "0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << static_cast< unsigned >(hp.value);

    os.fill(oldfill);
    os.flags(f);
    return os;
}

inline HexPrinter asHex(uint8_t v) { return HexPrinter{ v }; }

}// namespace Print