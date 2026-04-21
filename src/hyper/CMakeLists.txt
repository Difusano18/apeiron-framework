# src/hyper/CMakeLists.txt
# libapeiron-hyper - Hyper-acceleration and time dilation

set(HYPER_SOURCES
    src/time_dilation.cpp
)

# TODO: Add snapshot_manager.cpp when implemented

add_library(apeiron-hyper ${HYPER_SOURCES})

target_include_directories(apeiron-hyper
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(apeiron-hyper
    PUBLIC
        apeiron-core
)

target_compile_features(apeiron-hyper PUBLIC cxx_std_20)

set_target_properties(apeiron-hyper PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME hyper
)
