# src/core/CMakeLists.txt
# libapeiron-core - Core consciousness engine

set(CORE_SOURCES
    src/consciousness/awareness_engine.cpp
    src/consciousness/reflection_module.cpp
    src/cognition/thought_generator.cpp
    src/emotion/affective_state.cpp
    src/memory/episodic_store.cpp
)

add_library(apeiron-core ${CORE_SOURCES})

target_include_directories(apeiron-core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(apeiron-core
    PUBLIC
        Threads::Threads
)

target_compile_features(apeiron-core PUBLIC cxx_std_20)

set_target_properties(apeiron-core PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME core
)

# Export symbols for Windows DLLs
if(WIN32)
    target_compile_definitions(apeiron-core PRIVATE APEIRON_CORE_BUILD)
endif()
