target_sources(apeiron-safety
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ethical_monitor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/value_alignment_engine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ethical_predictor.cpp
)

target_include_directories(apeiron-safety
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
