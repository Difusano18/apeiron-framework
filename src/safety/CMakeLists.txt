set(SAFETY_SOURCES
    src/safety_monitor.cpp
    src/goal_aligner.cpp
    src/human_interface.cpp
    src/formal_verifier.cpp
)

add_library(apeiron-safety ${SAFETY_SOURCES})

add_subdirectory(ethics)

target_include_directories(apeiron-safety
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(apeiron-safety PUBLIC apeiron-core apeiron-mutation)
target_compile_definitions(apeiron-safety PRIVATE APEIRON_SAFETY_BUILD)
target_compile_features(apeiron-safety PUBLIC cxx_std_20)

set_target_properties(apeiron-safety PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME safety
)
