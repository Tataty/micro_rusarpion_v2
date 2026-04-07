#pragma once

#include <fmt/core.h>
#include <exception>
#include <string_view>

class InvalidParameterError : public std::exception {
public:
    template < typename T >
    InvalidParameterError(std::string_view parameterName, const T& parameterValue, std::string_view rule) {
        message = fmt::format("Parameter:  \"{}\" = {} | Rule: {}", parameterName, parameterValue, rule);
    }

    const char* what() const noexcept override { return message.c_str(); }

private:
    std::string message;
};