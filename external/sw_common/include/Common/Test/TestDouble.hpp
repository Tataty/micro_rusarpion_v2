#pragma once
#include <gtest/gtest.h>

static constexpr double ABS_ERROR = 0.0001;

#define ASSERT_NEAR_DOUBLE(expected, actual) ASSERT_NEAR(expected, actual, ABS_ERROR)