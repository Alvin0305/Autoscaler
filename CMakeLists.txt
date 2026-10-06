cmake_minimum_required(VERSION 3.16)

project(AutoScaler
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBVIRT REQUIRED libvirt)

add_executable(autoscaler
    src/main.cpp
    src/VMManager.cpp
    src/VMMonitor.cpp
    src/VM.cpp
    src/utils/utils.cpp    
)

target_include_directories(autoscaler
    PRIVATE
        include
        ${LIBVIRT_INCLUDE_DIRS}
)

target_link_directories(autoscaler
    PRIVATE
        ${LIBVIRT_LIBRARY_DIRS}
)

target_link_libraries(autoscaler
    PRIVATE
        ${LIBVIRT_LIBRARIES}
)

target_compile_options(autoscaler
    PRIVATE
        ${LIBVIRT_CFLAGS_OTHER}
)
