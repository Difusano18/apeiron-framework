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
install(TARGETS apeiron-core apeiron-cli
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY src/core/include/apeiron
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
