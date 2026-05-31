set(CONNECT_SOURCES
    src/llm_adapter.cpp
    src/mock_llm_adapter.cpp
    src/tool_executor.cpp
)

add_library(apeiron-connect ${CONNECT_SOURCES})

target_include_directories(apeiron-connect
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_definitions(apeiron-connect PRIVATE APEIRON_CONNECT_BUILD)
target_compile_features(apeiron-connect PUBLIC cxx_std_20)

set_target_properties(apeiron-connect PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME connect
)
