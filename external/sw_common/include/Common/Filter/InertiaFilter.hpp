#pragma once

#include "iFilter.hpp"

template < typename T > class InertiaFilter : public iFilter< T > {
private:
    double inertia;

    void updateFilterValue(T newValue) override { this->value = this->value * inertia + newValue * (1 - inertia); }

public:
    InertiaFilter(bool ignoreFirstValue, double inertia) : iFilter< T >(ignoreFirstValue), inertia(inertia) {}
};