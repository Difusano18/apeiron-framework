# APEIRON Framework

**Autonomous Emergent Intelligence Platform**

APEIRON is a commercial-grade C++ framework for creating self-evolving, emergent consciousness systems. It combines modular architecture, hyper-acceleration capabilities, and safe self-modification.

## Features

- **Core Modules**: Consciousness, cognition, emotion, memory
- **Hyper-Acceleration**: Subjective time dilation (1x to 1,000,000x)
- **Self-Modification**: Safe code mutation with version tree
- **Visualization**: Real-time 3D consciousness viewer (ImGui + OpenGL/Vulkan)
- **Sandboxing**: Docker/container isolation with resource limits
- **Native GUI Launcher**: One-step Windows GUI build and launch via `run.bat`

## Quick Start

```bash
# Clone and build
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON
make -j$(nproc)

# Run tests
./tests/apeiron-tests

# Run CLI
./src/cli/apeiron-cli --help
```

On Windows, run:

```bat
run.bat
```

This configures, builds, and launches the native Apeiron GUI directly.

## Architecture

```
libapeiron-core       # Consciousness, cognition, emotion, memory
libapeiron-hyper      # Time dilation, acceleration profiles
libapeiron-mutation   # Self-modification, version tree
libapeiron-world      # Entity system, physics, environments
libapeiron-viz        # Visualization, ImGui, OpenGL/Vulkan
libapeiron-connect    # LLM adapters, containers
libapeiron-learning   # Knowledge graph, reward tracking, model updates
libapeiron-safety     # Safety telemetry, goal filtering, verification stubs
libapeiron-distributed # Local distributed abstractions
apeiron-gui           # Native GUI monitor
```

## Roadmap

- **Foundation**: Core, hyper, viz, CLI, mutation, connect, world, learning, safety, distributed, GUI.
- **Phase 3 Mega Plan**: Autonomy, discovery, swarm coordination, recursive evolution, prediction, ethics, and GUI enhancements.

See [docs/phase3_mega_plan.md](docs/phase3_mega_plan.md).

## License

Dual License:
- **MIT**: Core library, CLI, basic visualization
- **Commercial**: Enterprise features (clustering, formal verification, support)

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for details.
