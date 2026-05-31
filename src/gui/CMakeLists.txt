set(GUI_ENHANCEMENT_SOURCES
    src/gui_manager.cpp
    src/visualization_panels.cpp
    src/control_panels.cpp
    src/debug_console.cpp
)

add_library(apeiron-gui-enhancements ${GUI_ENHANCEMENT_SOURCES})

target_include_directories(apeiron-gui-enhancements
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(apeiron-gui-enhancements
    PUBLIC
        apeiron-core
        apeiron-hyper
        apeiron-autonomy
        apeiron-discovery
        apeiron-swarm
        apeiron-evolution
        apeiron-prediction
        apeiron-safety
        apeiron-distributed
)

target_compile_definitions(apeiron-gui-enhancements PRIVATE APEIRON_GUI_BUILD)
target_compile_features(apeiron-gui-enhancements PUBLIC cxx_std_20)

set_target_properties(apeiron-gui-enhancements PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME gui-enhancements
)

add_executable(apeiron-gui WIN32
    src/main.cpp
)

target_link_libraries(apeiron-gui
    PRIVATE
        apeiron-core
        apeiron-hyper
        apeiron-viz
        apeiron-mutation
        apeiron-connect
        apeiron-world
        apeiron-learning
        apeiron-safety
        apeiron-distributed
        apeiron-gui-enhancements
)

target_compile_features(apeiron-gui PRIVATE cxx_std_20)

set_target_properties(apeiron-gui PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
