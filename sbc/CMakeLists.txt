cmake_minimum_required(VERSION 3.22)

#- PROJECT -#
project(micro_rusarpion_sbc VERSION 0.0.1 LANGUAGES CXX)

#- LOG -#
function(SBC_LOG status msg)
    message("{" ${PROJECT_NAME} "} (" ${status} ") [SBC] ${msg}")
endfunction()

#- CPP -#
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#- BUILD TYPE -#
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    SBC_LOG(STATUS "Debug build")
    add_compile_options("-Wall" "-Wno-error")

elseif(CMAKE_BUILD_TYPE STREQUAL "Release" OR NOT CMAKE_BUILD_TYPE)
    SBC_LOG(STATUS "Release build (or default)")
    add_compile_options("-march=native")

else()
    SBC_LOG(FATAL_ERROR "CMAKE_BUILD_TYPE is unknown! Value: '${CMAKE_BUILD_TYPE}'")
endif()

#- OPTIONS -#
set(CMAKE_POSITION_INDEPENDENT_CODE ON) 

configure_file(${CMAKE_SOURCE_DIR}/include/Config/ProjectVersion.hpp.in ProjectVersion.hpp @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#- SUBCMAKE -#
add_subdirectory(external/fmt)
add_subdirectory(external/json)
add_subdirectory(external/termcolor)
add_subdirectory(external/argparse)
add_subdirectory(external/sw_common)
add_subdirectory(${CMAKE_SOURCE_DIR}/src)