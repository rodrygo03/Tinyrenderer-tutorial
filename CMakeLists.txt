cmake_minimum_required(VERSION 3.22.1)
set (CMAKE_CXX_STANDARD 17)
project(tinyrenderer)


set(SOURCE_FILES
    src/vendor/tgaimage/tgaimage.cpp
    src/vendor/model/geometry.h
    src/vendor/model/model.cpp
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
