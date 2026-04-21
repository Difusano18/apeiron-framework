# src/cli/CMakeLists.txt
# apeiron-cli - Command line interface

add_executable(apeiron-cli
    src/main.cpp
)

target_link_libraries(apeiron-cli
    PRIVATE
        apeiron-core
        apeiron-hyper
        apeiron-viz
)

target_compile_features(apeiron-cli PRIVATE cxx_std_20)

set_target_properties(apeiron-cli PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
