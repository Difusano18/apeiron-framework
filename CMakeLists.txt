cmake_minimum_required(VERSION 3.25)
project(apeiron
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "Autonomous Emergent Intelligence Framework"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Place all binaries in the same bin/ directory so DLL dependency resolution works on Windows
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Options
option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(ENABLE_WARNINGS "Enable compiler warnings" ON)

# Compiler warnings
if(ENABLE_WARNINGS)
    if(MSVC)
        add_compile_options(/W4 /WX)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    endif()
endif()

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

# Find packages
find_package(Threads REQUIRED)

# Core library and CLI
add_subdirectory(src)

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Install rules
include(GNUInstallDirs)
install(TARGETS
    apeiron-core
    apeiron-hyper
    apeiron-viz
    apeiron-mutation
    apeiron-connect
    apeiron-world
    apeiron-learning
    apeiron-safety
    apeiron-distributed
    apeiron-autonomy
    apeiron-prediction
    apeiron-discovery
    apeiron-swarm
    apeiron-evolution
    apeiron-gui-enhancements
    apeiron-cli
    apeiron-gui
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY src/core/include/apeiron
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY
    src/hyper/include/apeiron
    src/viz/include/apeiron
    src/mutation/include/apeiron
    src/connect/include/apeiron
    src/world/include/apeiron
    src/learning/include/apeiron
    src/safety/include/apeiron
    src/distributed/include/apeiron
    src/autonomy/include/apeiron
    src/prediction/include/apeiron
    src/discovery/include/apeiron
    src/swarm/include/apeiron
    src/evolution/include/apeiron
    src/gui/include/apeiron
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
