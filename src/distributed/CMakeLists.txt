set(DISTRIBUTED_SOURCES
    src/cluster_manager.cpp
    src/message_broker.cpp
    src/task_scheduler.cpp
    src/data_synchronizer.cpp
)

add_library(apeiron-distributed ${DISTRIBUTED_SOURCES})

target_include_directories(apeiron-distributed
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_definitions(apeiron-distributed PRIVATE APEIRON_DISTRIBUTED_BUILD)
target_compile_features(apeiron-distributed PUBLIC cxx_std_20)

set_target_properties(apeiron-distributed PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME distributed
)
