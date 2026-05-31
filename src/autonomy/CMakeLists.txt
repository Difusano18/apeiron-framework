set(AUTONOMY_SOURCES
    src/goal_manager.cpp
    src/intrinsic_motivation_engine.cpp
    src/goal_evaluator.cpp
)

add_library(apeiron-autonomy ${AUTONOMY_SOURCES})

target_include_directories(apeiron-autonomy
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(apeiron-autonomy
    PUBLIC
        apeiron-core
        apeiron-learning
        apeiron-safety
        apeiron-world
        apeiron-connect
)

target_compile_definitions(apeiron-autonomy PRIVATE APEIRON_AUTONOMY_BUILD)
target_compile_features(apeiron-autonomy PUBLIC cxx_std_20)

set_target_properties(apeiron-autonomy PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME autonomy
)
