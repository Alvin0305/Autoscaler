add_executable(autoscaler-client
    src/main.cpp
    
    src/core/Client.cpp
    src/core/LoadBalancer.cpp

    src/tools/WorkGenerator.cpp
)

target_include_directories(autoscaler-client
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(autoscaler-client
    PRIVATE
        autoscaler-common
)