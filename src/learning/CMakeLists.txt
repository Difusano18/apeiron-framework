set(LEARNING_SOURCES
    src/knowledge_graph.cpp
    src/reinforcement_learner.cpp
    src/model_updater.cpp
    src/learning_engine.cpp
)

add_library(apeiron-learning ${LEARNING_SOURCES})

target_include_directories(apeiron-learning
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(apeiron-learning PUBLIC apeiron-core apeiron-world)
target_compile_definitions(apeiron-learning PRIVATE APEIRON_LEARNING_BUILD)
target_compile_features(apeiron-learning PUBLIC cxx_std_20)

set_target_properties(apeiron-learning PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME learning
)
