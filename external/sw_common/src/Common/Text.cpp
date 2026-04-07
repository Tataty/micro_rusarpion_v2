#include <algorithm>
#include "Common/Text.hpp"

std::u16string utf8To16(const std::string& utf8) {
    std::wstring_convert< std::codecvt_utf8_utf16< char16_t >, char16_t > convert;
    return convert.from_bytes(utf8);
}

std::string utf16To8(const std::u16string& utf16) {
    std::wstring_convert< std::codecvt_utf8_utf16< char16_t >, char16_t > convert;
    return convert.to_bytes(utf16);
}

std::string toLower(const std::string& text) {
    std::string result = text;
    std::transform(text.begin(), text.end(), result.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return result;
}

std::u16string toUpper(const std::u16string& text) {
    std::u16string result;
    for (char16_t ch : text) {
        if (ch >= u'а' && ch <= u'я')
            result += ch - (u'а' - u'А');
        else
            result += std::towupper(ch);
    }
    return result;
}

std::string addSpacesAfterLetters(const std::string& text) {
    std::string result;
    for (size_t i = 0; i < text.length(); ++i) {
        result += text[i];
        if (i == text.size() - 1)
            continue;
        if (text[i] == '.' || (i + 1 < text.length() && text[i + 1] == '.'))
            continue;
        result += ' ';
    }
    return result;
}

std::u16string addSpacesAfterLetters(const std::u16string& text) {
    std::u16string result;
    for (size_t i = 0; i < text.length(); ++i) {
        result += text[i];
        if (text[i] == u'.' || (i + 1 < text.length() && text[i + 1] == u'.'))
            continue;
        result += u' ';
    }
    return result;
}

std::string numberToDuplicateString(int number, int duplicate) {
    std::stringstream ss;
    if (number < 0) {
        ss << '-';
        number = std::abs(number);
    }
    ss << std::setw(duplicate) << std::setfill('0') << number;
    return ss.str();
}

bool replaceAll(std::string& source, const std::string& from, const std::string& to) {
    bool wasReplace = false;

    std::string::size_type pos = 0;
    while ((pos = source.find(from, pos)) != std::string::npos) {
        source.replace(pos, from.length(), to);
        pos += to.length();
        wasReplace = true;
    }

    return wasReplace;
}