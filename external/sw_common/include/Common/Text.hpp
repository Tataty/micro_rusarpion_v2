#pragma once

#include <algorithm>
#include <cctype>
#include <codecvt>
#include <cwctype>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

std::u16string utf8To16(const std::string& utf8);
std::string    utf16To8(const std::u16string& utf16);

std::string toLower(const std::string& text);

std::string    toUpper(const std::string& text);
std::u16string toUpper(const std::u16string& text);

std::string    addSpacesAfterLetters(const std::string& text);
std::u16string addSpacesAfterLetters(const std::u16string& text);

std::string numberToDuplicateString(int number, int duplicate);

bool replaceAll(std::string& source, const std::string& from, const std::string& to);
