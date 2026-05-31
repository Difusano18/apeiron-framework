set(WORLD_SOURCES
    src/environment.cpp
    src/grid_environment.cpp
)

add_library(apeiron-world ${WORLD_SOURCES})

target_include_directories(apeiron-world
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(apeiron-world PUBLIC apeiron-core)
target_compile_definitions(apeiron-world PRIVATE APEIRON_WORLD_BUILD)
target_compile_features(apeiron-world PUBLIC cxx_std_20)

set_target_properties(apeiron-world PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME world
)
