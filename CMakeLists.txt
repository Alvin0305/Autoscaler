cmake_minimum_required(VERSION 3.16)

project(AutoScaler
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(app/common)
add_subdirectory(app/server)
add_subdirectory(app/client)
