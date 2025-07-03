# SimpleOBS Architecture

## Overview

SimpleOBS is a simplified implementation of OBS Studio, designed for learning and understanding the core architecture of streaming software.

## Core Architecture

### 1. Engine (Core)
- **Location**: `src/core/`
- **Purpose**: Main application engine, manages all components
- **Key Classes**:
  - `Engine`: Singleton main engine
  - `SceneImpl`: Scene implementation
  - `VideoFrame`: Video frame structure
  - `AudioFrame`: Audio frame structure

### 2. Sources
- **Location**: `src/sources/`
- **Purpose**: Video and audio sources
- **Key Classes**:
  - `Source`: Base source interface
  - `BaseSource`: Basic source implementation

### 3. Encoders
- **Location**: `src/encoders/`
- **Purpose**: Video and audio encoding
- **Key Classes**:
  - `Encoder`: Base encoder interface
  - `X264Encoder`: H.264 encoder (placeholder)

### 4. Outputs
- **Location**: `src/outputs/`
- **Purpose**: Streaming and recording outputs
- **Key Classes**:
  - `Output`: Base output interface
  - `RTMPOutput`: RTMP streaming (placeholder)

### 5. Filters
- **Location**: `src/filters/`
- **Purpose**: Video and audio filters
- **Key Classes**:
  - `Filter`: Base filter interface
  - `CropFilter`: Video cropping (placeholder)
  - `ScaleFilter`: Video scaling (placeholder)

## Design Patterns

### 1. Interface + Implementation Separation
- All major components use interface + implementation pattern
- Example: `Scene` (interface) + `SceneImpl` (implementation)
- Enables easy extension and testing

### 2. Singleton Pattern
- `Engine` uses singleton pattern for global access
- Ensures single instance throughout application lifecycle

### 3. Factory Pattern
- `Engine` acts as factory for creating sources, encoders, outputs, filters
- Centralized object creation with type-based instantiation

## Data Flow

```
Sources → Scene → Encoders → Outputs
   ↓         ↓        ↓        ↓
Filters → Scene → Encoders → Outputs
```

1. **Sources** generate video/audio frames
2. **Scene** manages and renders sources
3. **Filters** process frames (optional)
4. **Encoders** compress frames
5. **Outputs** stream/record encoded data

## Threading Model

- **Main Thread**: UI and control operations
- **Streaming Thread**: Dedicated thread for streaming loop
- **Source Threads**: Individual threads for each source (future)
- **Encoder Threads**: Dedicated threads for encoding (future)

## Extension Points

### Adding New Sources
1. Create new class inheriting from `Source`
2. Implement required virtual methods
3. Register in `Engine::createSource()`

### Adding New Encoders
1. Create new class inheriting from `Encoder`
2. Implement encoding logic
3. Register in `Engine::createEncoder()`

### Adding New Outputs
1. Create new class inheriting from `Output`
2. Implement output logic
3. Register in `Engine::createOutput()`

### Adding New Filters
1. Create new class inheriting from `Filter`
2. Implement processing logic
3. Register in `Engine::createFilter()`

## Future Enhancements

1. **Plugin System**: Dynamic loading of components
2. **Configuration System**: JSON-based settings
3. **UI Framework**: Qt-based user interface
4. **Network Layer**: WebRTC, HLS support
5. **Hardware Acceleration**: GPU encoding/decoding 