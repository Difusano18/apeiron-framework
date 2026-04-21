# src/CMakeLists.txt
# Builds all libraries and executables

# Core library
add_subdirectory(core)

# Hyper-acceleration library (Phase 2)
add_subdirectory(hyper)

# Visualization library (Phase 3)
add_subdirectory(viz)

# CLI executable
add_subdirectory(cli)
