set(PREDICTION_SOURCES
    src/predictive_model.cpp
    src/causal_inference_engine.cpp
    src/future_simulator.cpp
)

add_library(apeiron-prediction ${PREDICTION_SOURCES})

target_include_directories(apeiron-prediction
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(apeiron-prediction PUBLIC apeiron-core apeiron-world apeiron-learning)
target_compile_definitions(apeiron-prediction PRIVATE APEIRON_PREDICTION_BUILD)
target_compile_features(apeiron-prediction PUBLIC cxx_std_20)

set_target_properties(apeiron-prediction PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME prediction
)
