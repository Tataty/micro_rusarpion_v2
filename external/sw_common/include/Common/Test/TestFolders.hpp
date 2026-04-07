#pragma once
#include <string>

const std::string               TEST_FOLDER_PATH = "local/";
static inline const std::string TEST_FILE(const std::string& x) { return TEST_FOLDER_PATH + x; }