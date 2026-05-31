set(SWARM_SOURCES
    src/agent_communicator.cpp
    src/task_delegator.cpp
    src/consensus_mechanism.cpp
)

add_library(apeiron-swarm ${SWARM_SOURCES})

target_include_directories(apeiron-swarm
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(apeiron-swarm PUBLIC apeiron-core apeiron-distributed apeiron-autonomy apeiron-safety apeiron-world)
target_compile_definitions(apeiron-swarm PRIVATE APEIRON_SWARM_BUILD)
target_compile_features(apeiron-swarm PUBLIC cxx_std_20)

set_target_properties(apeiron-swarm PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME swarm
)
