cmake_minimum_required(VERSION 3.22)

#- PROJECT -#
project(sw_common VERSION 1.2.0 LANGUAGES CXX)

#- LOG -#
function(DONS_LOG status msg)
    message("{" ${PROJECT_NAME} "} (" ${status} ") [DONS] ${msg}")
endfunction()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fix undefined uintX_t error #
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include cstdint")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#- SUBCMAKE -#
add_subdirectory(src)