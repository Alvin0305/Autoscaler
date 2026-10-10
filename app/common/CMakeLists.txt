add_library(autoscaler-common STATIC
    src/logger/Logger.cpp

    src/model/Message.cpp
    src/model/Work.cpp

    src/utils/utils.cpp
    src/utils/RandomGenerator.cpp
)

target_include_directories(autoscaler-common
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)