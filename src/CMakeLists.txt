# src/CMakeLists.txt
# Builds all libraries and executables

# Core library
add_subdirectory(core)

# Hyper-acceleration library (Phase 2)
add_subdirectory(hyper)

# Visualization library (Phase 3)
add_subdirectory(viz)

# Compile-first extension libraries
add_subdirectory(mutation)
add_subdirectory(connect)
add_subdirectory(world)
add_subdirectory(learning)
add_subdirectory(safety)
add_subdirectory(distributed)
add_subdirectory(autonomy)
add_subdirectory(prediction)
add_subdirectory(discovery)
add_subdirectory(swarm)
add_subdirectory(evolution)

# CLI executable
add_subdirectory(cli)

# Native GUI executable
add_subdirectory(gui)
