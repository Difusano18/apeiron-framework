# tests/CMakeLists.txt

if(NOT TARGET Catch2::Catch2)
    # Fetch Catch2 if not available
    Include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.2
    )
    FetchContent_MakeAvailable(Catch2)
endif()

set(TEST_SOURCES
    unit/test_awareness.cpp
    unit/test_thoughts.cpp
    unit/test_emotion.cpp
    unit/test_memory.cpp
    unit/test_bridge.cpp
    unit/test_hyperloop.cpp
    unit/test_mutation.cpp
    unit/test_connect.cpp
    unit/test_world.cpp
    unit/test_learning.cpp
    unit/test_safety.cpp
    unit/test_distributed.cpp
    unit/test_extension_integration.cpp
    unit/test_autonomy.cpp
    unit/test_prediction.cpp
    unit/test_discovery.cpp
    unit/test_swarm.cpp
    unit/test_evolution.cpp
    unit/test_ethics.cpp
    unit/test_gui.cpp
)

add_executable(apeiron-tests ${TEST_SOURCES})

target_link_libraries(apeiron-tests
    PRIVATE
        apeiron-core
        apeiron-hyper
        apeiron-mutation
        apeiron-connect
        apeiron-world
        apeiron-learning
        apeiron-safety
        apeiron-distributed
        apeiron-autonomy
        apeiron-prediction
        apeiron-discovery
        apeiron-swarm
        apeiron-evolution
        apeiron-gui-enhancements
        Catch2::Catch2WithMain
)

target_compile_features(apeiron-tests PRIVATE cxx_std_20)

# Register with CTest
include(Catch)
catch_discover_tests(apeiron-tests)
