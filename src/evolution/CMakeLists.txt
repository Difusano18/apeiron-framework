set(EVOLUTION_SOURCES
    src/performance_profiler.cpp
    src/meta_optimizer.cpp
    src/architecture_designer.cpp
)

add_library(apeiron-evolution ${EVOLUTION_SOURCES})

target_include_directories(apeiron-evolution
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(apeiron-evolution PUBLIC apeiron-core apeiron-mutation apeiron-learning apeiron-safety apeiron-hyper)
target_compile_definitions(apeiron-evolution PRIVATE APEIRON_EVOLUTION_BUILD)
target_compile_features(apeiron-evolution PUBLIC cxx_std_20)

set_target_properties(apeiron-evolution PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME evolution
)
