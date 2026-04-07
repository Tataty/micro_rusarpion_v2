#pragma once

#include <libudev.h>
#include <sys/stat.h>
#include <optional>
#include <stdexcept>

std::optional< std::string > findAudioALSAByCamera(const std::string& symlink_path);