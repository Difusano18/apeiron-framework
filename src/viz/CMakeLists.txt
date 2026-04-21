# src/viz/CMakeLists.txt
# libapeiron-viz - Visualization and rendering

set(VIZ_SOURCES
    src/console_renderer.cpp
)

add_library(apeiron-viz ${VIZ_SOURCES})

target_include_directories(apeiron-viz
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(apeiron-viz
    PUBLIC
        apeiron-core
        apeiron-hyper
)

target_compile_features(apeiron-viz PUBLIC cxx_std_20)

set_target_properties(apeiron-viz PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME viz
)
