set(MUTATION_SOURCES
    src/mutation_engine.cpp
    src/code_analyzer.cpp
    src/mutation_proposer.cpp
    src/safety_validator.cpp
    src/version_control.cpp
)

add_library(apeiron-mutation ${MUTATION_SOURCES})

target_include_directories(apeiron-mutation
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(apeiron-mutation PUBLIC apeiron-core)
target_compile_definitions(apeiron-mutation PRIVATE APEIRON_MUTATION_BUILD)
target_compile_features(apeiron-mutation PUBLIC cxx_std_20)

set_target_properties(apeiron-mutation PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME mutation
)
