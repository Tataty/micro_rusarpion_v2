cmake_minimum_required(VERSION 3.22)

#- PROJECT -#
project(dons_turret_jetson VERSION 1.0.0 LANGUAGES CXX)

#- LOG -#
function(DONS_LOG status msg)
    message("{" ${PROJECT_NAME} "} (" ${status} ") [DONS] ${msg}")
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
    DONS_LOG(STATUS "Debug build")
    add_compile_options("-Wall" "-Wno-error")

elseif(CMAKE_BUILD_TYPE STREQUAL "Release" OR NOT CMAKE_BUILD_TYPE)
    DONS_LOG(STATUS "Release build (or default)")
    add_compile_options("-march=native")

else()
    DONS_LOG(FATAL_ERROR "CMAKE_BUILD_TYPE is unknown! Value: '${CMAKE_BUILD_TYPE}'")
endif()

#- ARCHITECTURE -#
if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm" OR CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    set(TARGET_PLATFORM_IS_JETSON TRUE)
    DONS_LOG(STATUS "Target platform: JETSON")
else()
    DONS_LOG(STATUS "Target platform: X86")
endif()

#- OPTIONS -#
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)
option(USING_GPU "GPU usage (Requires TensorRT & CUDA)" ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON) 

configure_file(${CMAKE_SOURCE_DIR}/include/Config/ProjectVersion.hpp.in ProjectVersion.hpp @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#- PACKAGE -#
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Eigen3 REQUIRED)

pkg_check_modules(GST REQUIRED gstreamer-1.0 gstreamer-plugins-base-1.0 gstreamer-app-1.0)
pkg_check_modules(FFMPEG REQUIRED libavcodec libavformat libavutil libswscale)

if(TARGET_PLATFORM_IS_JETSON) # --- JETSON --- #
    #- VERSION -#
    option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
    set(BUILD_SHARED_LIBS OFF)

    # TensorRT
    find_library(NVINFER_LIB nvinfer)
    find_library(NVINFER_PLUGIN_LIB nvinfer_plugin)
    
    set(TENSORRT_LIBRARIES ${NVINFER_LIB} ${NVINFER_PLUGIN_LIB})
    set(TENSORRT_INCLUDE_DIRS /usr/include/aarch64-linux-gnu)

    find_package(CUDA REQUIRED)
    find_package(gflags REQUIRED)
    find_package(glog REQUIRED)
    find_package(Iconv REQUIRED)

    include_directories(
        ${gflags_INCLUDE_DIR}
    )
    
else() # --- PC --- #
    # TensorRT
    set(TENSORRT_INCLUDE_DIRS ${TENSORRT_ROOT}/include)
    set(TENSORRT_LIBRARIES ${TENSORRT_ROOT}/lib/libnvinfer.so)
    
    # Cuda
    set(CUDA_INCLUDE_DIRS ${CUDA_ROOT}/include)
    set(CUDA_LIBRARIES ${CUDA_ROOT}/lib64/libcudart.so)
endif()

#- INCLUDE -#
include_directories(
    ${EIGEN3_INCLUDE_DIR} 
    ${OpenCV_INCLUDE_DIRS}
    ${GST_INCLUDE_DIRS}
    ${GST_APP_INCLUDE_DIRS}
    ${GST_BASE_INCLUDE_DIRS}
)

#- SUBCMAKE -#
add_subdirectory(external/sw_common)
add_subdirectory(external/univision_sdk)
add_subdirectory(external/argparse)
add_subdirectory(external/yolotrack)
add_subdirectory(src)

# if(BUILD_TESTS)
#     DONS_LOG(STATUS "Build tests")
#     enable_testing()
#     add_subdirectory(tests)
# endif()

if(BUILD_EXAMPLES)
    DONS_LOG(STATUS "Build examples")
    add_subdirectory(examples)
endif()