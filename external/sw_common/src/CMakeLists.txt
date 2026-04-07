#- VARIABLE -#
set(PROJECT Common)
set(PROJECT_LIB_CONFIG ${PROJECT}LibConfig)
set(PROJECT_TARGETS ${PROJECT}Targets)
set(PROJECT_LIB ${PROJECT}Lib)

file(GLOB_RECURSE SRC_FILES *.cpp)

find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui imgcodecs freetype)

pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

#- LIBRARY -#
target_link_libraries(${PROJECT_NAME} 
    PUBLIC  
        opencv_core
        opencv_imgproc
        opencv_highgui
        opencv_imgcodecs
        opencv_freetype
        fmt::fmt
        nlohmann_json::nlohmann_json 
        termcolor::termcolor
        ${LIBUSB_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        ${LIBUSB_INCLUDE_DIRS}
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#- INCLUDE -#
install(TARGETS ${PROJECT_NAME} fmt nlohmann_json termcolor
    EXPORT ${PROJECT_TARGETS}
    LIBRARY DESTINATION lib  # .so, .dll, .dylib
    ARCHIVE DESTINATION lib  # .a, .lib
    PUBLIC_HEADER DESTINATION include
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ 
    DESTINATION include
    PATTERN "*/"
    PATTERN "*.h"
    PATTERN "*.hpp"
)

install(EXPORT ${PROJECT_TARGETS}
    FILE ${PROJECT_LIB_CONFIG}.cmake
    NAMESPACE ${PROJECT}::
    DESTINATION lib/cmake/${PROJECT_LIB}
)