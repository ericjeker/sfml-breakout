# TODO

## Managers

- [x] ServiceLocator
- [x] Scene Loading
- [x] EventSystem

## Features

- [ ] Entity, Component, System Factories
- [ ] Load scenes, entities, components, systems from serialized files

## General Optimizations

### 1. Rendering Optimizations

- Batch rendering: Instead of drawing each ball individually, use `sf::VertexArray` or `sf::VertexBuffer` to draw
  multiple balls in a single draw call
- Culling: Only render balls that are visible on screen
- Sprite batching: Use texture atlases and batch similar sprites together

### 2. Physics Optimizations

- Spatial partitioning: Use a spatial hash grid, quadtree, or broad-phase collision detection
- Object pooling: Reuse ball objects instead of creating/destroying them
- Fixed timestep: Use a fixed timestep for physics calculations to improve consistency

### 3. ECS Optimizations

- Component data layout: Use Structure of Arrays (SoA) instead of Array of Structures (AoS) for better cache
  locality
- System batching: Process components in batches to improve cache efficiency
- Memory alignment: Ensure components are properly aligned in memory

### 4. SFML-Specific Optimizations

- Reduce state changes: Minimize texture binding and shader changes
- Use sf::Transform: Cache transformation matrices instead of recalculating them
- Profile with sf::Clock: Measure actual frame times to identify bottlenecks

### 5. General C++ Optimizations

- Avoid dynamic allocation: Use object pools and stack allocation where possible
- Profile-guided optimization: Use profilers to identify hotspots
- Compiler flags: Use `-O3` and appropriate optimization flags

## Games

### 1. Bouncing Ball Animation

- Simple physics simulation with a ball bouncing around the screen
- Tests: Basic rendering, animation system, collision detection with screen boundaries
- Great for showcasing smooth movement and your animation framework

Gradually add features like:

- Multiple balls with different properties
- Color changes on collision
- Particle trails
- Sound effects
- Interactive elements (mouse interaction)

### 2. Simple Snake Game

- Classic snake that grows when eating food
- Tests: Input handling, game state management, collision detection, score tracking
- Uses basic sprites and grid-based movement

### 3. Asteroid Field

- Player spaceship navigating through floating asteroids
- Tests: Sprite rotation, collision detection, particle effects, input responsiveness
- Can showcase smooth movement and rotation animations

### 4. Simple Platformer

- Basic character jumping on platforms
- Tests: Gravity simulation, collision detection, sprite animation, level management
- Great for testing scene management with different levels

### 5. Pong Clone

- Two paddles hitting a ball back and forth
- Tests: Input handling, collision detection, score system, AI for single player
- Classic and easy to implement
