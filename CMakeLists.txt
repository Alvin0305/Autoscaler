cmake_minimum_required(VERSION 3.16)

project(AutoScaler
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBVIRT REQUIRED libvirt)

find_package(LibXml2 REQUIRED)
pkg_check_modules(LIBXMLXX REQUIRED IMPORTED_TARGET libxml++-2.6)

add_executable(autoscaler
    src/main.cpp
    src/core/AutoScaler.cpp
    src/core/VMManager.cpp
    src/core/VMMonitor.cpp
    src/scaler/VMCloner.cpp
    src/scaler/VMDestroyer.cpp
    src/tools/XMLParser.cpp
    src/connection/MessageSender.cpp
    src/connection/Socket.cpp
    src/VM.cpp
    src/logger/Logger.cpp
    src/utils/utils.cpp
    src/utils/IDGenerator.cpp
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
        PkgConfig::LIBXMLXX
)

target_compile_options(autoscaler
    PRIVATE
        ${LIBVIRT_CFLAGS_OTHER}
)
