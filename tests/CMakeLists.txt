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
)

add_executable(apeiron-tests ${TEST_SOURCES})

target_link_libraries(apeiron-tests
    PRIVATE
        apeiron-core
        Catch2::Catch2WithMain
)

target_compile_features(apeiron-tests PRIVATE cxx_std_20)

# Register with CTest
include(Catch)
catch_discover_tests(apeiron-tests)
