cmake_minimum_required(VERSION 3.10.0)
project(bmpIO VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(
        /W4
        /WX
        /wd4201
        /wd4251
        /wd4275)
else()
    add_compile_options(
        -Wall
        -Wextra
        -Werror
        -Wpedantic
        -Wconversion)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src)

set(BMPIOSOURCE
    src/BMPProcessor.hpp
    src/BMPProcessor.cpp
)

add_library(BMPProcessor STATIC ${BMPIOSOURCE})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} BMPProcessor)

