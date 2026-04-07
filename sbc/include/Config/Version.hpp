#pragma once

#include "Common/Type/Version.hpp"
#include "ProjectVersion.hpp"

static constexpr int VERSION_MAJOR = PROJECT_VERSION_MAJOR;
static constexpr int VERSION_MINOR = PROJECT_VERSION_MINOR;
static constexpr int VERSION_PATCH = PROJECT_VERSION_PATCH;

static Version PROJECT_VERSION{ VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH };