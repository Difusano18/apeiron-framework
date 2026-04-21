# APEIRON Architecture

## Overview

APEIRON is a modular C++ framework for emergent consciousness systems.

## Core Modules

### libapeiron-core

#### Consciousness
- **AwarenessEngine**: Tracks awareness level (0.0-1.0) and stage progression
- **ReflectionModule**: Analyzes thoughts for self-reflection qualities

#### Cognition
- **ThoughtGenerator**: Generates thoughts based on context and awareness

#### Emotion
- **AffectiveState**: Manages emotional valence and arousal

#### Memory
- **EpisodicStore**: Stores experiences with associative retrieval

## Design Patterns

### Module Structure
Each module follows the pattern:
```
module/
├── include/apeiron/module/
│   └── *.h          # Public headers
└── src/
    └── *.cpp        # Implementation
```

### Thread Safety
- Core state uses `std::atomic` for lock-free access
- Heavy computation isolated to worker threads
- Main thread handles visualization and I/O

### Memory Management
- Custom allocators for hot paths
- Memory pools for frequent allocations
- RAII throughout

## Build System

CMake 3.25+ with modular targets:
- `apeiron-core`: Shared library
- `apeiron-cli`: Executable
- `apeiron-tests`: Test suite

## Testing

Catch2 v3 with:
- Unit tests per module
- Integration tests for workflows
- Benchmarks for performance regression

## Roadmap

See main README.md for development phases.
