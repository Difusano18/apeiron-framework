set(DISCOVERY_SOURCES
    src/hypothesis_generator.cpp
    src/experiment_designer.cpp
    src/data_analyzer.cpp
    src/theory_builder.cpp
)

add_library(apeiron-discovery ${DISCOVERY_SOURCES})

target_include_directories(apeiron-discovery
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(apeiron-discovery PUBLIC apeiron-core apeiron-world apeiron-learning apeiron-connect apeiron-prediction)
target_compile_definitions(apeiron-discovery PRIVATE APEIRON_DISCOVERY_BUILD)
target_compile_features(apeiron-discovery PUBLIC cxx_std_20)

set_target_properties(apeiron-discovery PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME discovery
)
