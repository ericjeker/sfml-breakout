# TODO

## Managers

- [x] ServiceLocator
- [x] SceneManager
- [x] EventSystem
- [ ] AudioManager
- [ ] CameraManager, CameraSystem, Scrolling, Culling (?)
- [ ] Reusable ObjectPool
- [ ] CommandRegistry, CommandSystem
- [ ] UIManager, GraphScene, SceneNode

## Features

- [ ] Entity, Component, System Factories
- [ ] Load scenes, entities, components, systems from serialized files
- [ ] CollisionSystem with collision shape (circle, rectangle)
- [ ] SpatialPartitioning (grid)
- [ ] SpriteRenderer
- [ ] ControllerSystem (PlayerController, AIController, NetworkController)
  - [ ] Behaviors (State Machine, Behavior Tree)
  - [ ] Possession
- [ ] AnimationSystem, SpriteAnimations
- [ ] ParticleSystem
- [ ] ShaderEffects

## Bugs / Improvements

- [x] ScreenBouncing doesn't account for drawable size
- [x] Integrate Tracy for profiling
- [ ] MainMenuState contains everything, it should be a game selector with an FSM

## Clean Code

- [x] Remove magic numbers in BouncingBallScene (radius, gravity, etc.)
- [ ] Error handling can certainly be improved everywhere


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
- Memory alignment: Ensure components are properly aligned in memory using a ComponentManager
- Filter entities: Systems should not loop all the entities/components, only the one they need

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

- [x] Simple physics simulation with a ball bouncing around the screen
- Tests: Basic rendering, animation system, collision detection with screen boundaries

#### Visual Enhancements

- Trail Effects: Add particle trails behind each ball that fade over time
- Color Variations: Make balls change color based on velocity, or cycle through colors
- Size Variations: Create balls of different sizes with different physics properties
- Lighting Effects: Add a simple bloom or glow effect around the balls

#### Interactive Features

- Mouse Interaction:
    - [x] Click to spawn new balls at the mouse position
    - [ ] Mouse repulsion/attraction forces
    - [x] Click and drag to create balls with initial velocity

- Keyboard Controls:
    - [x] Toggle gravity on/off
    - [ ] Adjust gravity strength
    - [x] Pause/resume simulation
    - [x] Reset scene

#### Physics Enhancements

- Ball-to-Ball Collisions: Implement elastic collisions between balls
- Different Materials: Give balls different bounce coefficients and friction
- Air Resistance: Add drag force that increases with velocity
- Magnetic/Electric Forces: Add attraction/repulsion between certain balls

#### Advanced Challenges

- Spatial Partitioning: Implement a quadtree or grid for efficient collision detection
- Performance Optimization: Profile and optimize to handle 1000+ balls smoothly
- Collision Response: Add realistic physics with conservation of momentum
- Constraints: Add springs or ropes connecting some balls
- Fluid Dynamics: Simulate basic fluid-like behavior when balls cluster

#### Game-like Features

- Score System: Points for ball interactions or achieving certain patterns
- Power-ups: Special balls that affect others when they collide
- Obstacles: Add static or moving obstacles in the scene
- Goals: Create targets or zones that balls need to reach


### 2. Simple Snake Game

#### Features

- Classic snake that grows when eating food
- Tests: Input handling, game state management, collision detection, score tracking
- Uses basic sprites and grid-based movement

#### Implementations

- `SimpleSnakeScene`

#### Improvements

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
