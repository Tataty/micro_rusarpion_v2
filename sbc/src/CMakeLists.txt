set(PROJECT_NAME_LIB ${PROJECT_NAME}_lib)

file(GLOB_RECURSE SRC_FILES *.cpp)
list(REMOVE_ITEM SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_library(${PROJECT_NAME_LIB} STATIC ${SRC_FILES})

#- INCLUDE -#
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)

#- LIBRARY -#
target_link_libraries(${PROJECT_NAME_LIB} 
    PUBLIC  
        atomic
        sw_common::sw_common
        Threads::Threads
        argparse::argparse
        ${OpenCV_LIBS} 
        ${GST_LIBRARIES}
        ${UDEV_LIBRARIES}
        ${GST_APP_LIBRARIES}
        ${GST_VIDEO_LIBRARIES}
        ${GST_ALLOC_LIBRARIES}
)

target_include_directories(${PROJECT_NAME_LIB} 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/include 
        ${GST_INCLUDE_DIRS}
        ${UDEV_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        ${GST_APP_INCLUDE_DIRS}
        ${GST_VIDEO_INCLUDE_DIRS}
        ${GST_ALLOC_INCLUDE_DIRS}
)

target_include_directories(${PROJECT_NAME_LIB} 
    SYSTEM PUBLIC 
        ${CMAKE_SOURCE_DIR}/external 
)

#- EXEC -#
add_executable(${PROJECT_NAME} main.cpp)

#- LIBRARY -#
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME_LIB})

#- COMPILE OPTIONS -#
target_compile_options(${PROJECT_NAME} PRIVATE -Wno-deprecated-declarations)
target_compile_options(${PROJECT_NAME_LIB} PRIVATE -Wno-deprecated-declarations)
target_compile_options(${PROJECT_NAME_LIB} PRIVATE -Wall -Wextra -pedantic -Werror)