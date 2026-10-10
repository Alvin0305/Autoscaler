find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBVIRT REQUIRED IMPORTED_TARGET libvirt)
pkg_check_modules(LIBXMLXX REQUIRED IMPORTED_TARGET libxml++-2.6)

add_executable(autoscaler-server
    src/main.cpp

    src/core/AutoScaler.cpp
    src/core/VMManager.cpp
    src/core/VMMonitor.cpp

    src/scaler/VMCloner.cpp
    src/scaler/VMDestroyer.cpp

    src/connection/MessageSender.cpp

    src/VM.cpp

    src/utils/IDGenerator.cpp
    src/tools/XMLParser.cpp
)

target_include_directories(autoscaler-server
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(autoscaler-server
    PRIVATE
        autoscaler-common
        PkgConfig::LIBVIRT
        PkgConfig::LIBXMLXX
)