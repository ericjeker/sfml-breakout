# TODO

## Architecture Cleanup

- [ ] Probably the namespacing sucks a little right now... Properly prefix with Core:: and Breakout::
- [ ] Configuration is in Core instead of being application-specific, use Config.json instead.
- [ ] Pixels-per-centimeter is in Core and cannot be configured from the app.
- [ ] Collision.cpp should be moved to the physics module instead.

### Flecs Specifics Cleanup

- [ ] All the systems should be in their own files.
- [ ] Some Prefabs are actually factories, they should be namespaced accordingly.
- [ ] All the KeyPressed Systems should be using a key binding instead of a hardcoded key.

## Bugs / Improvements

- [x] ScreenBouncing doesn't account for drawable size
- [x] Integrate Tracy for profiling
- [x] MenuState contains everything, it should be a game selector with a FSM
- [x] Use namespaces to encapsulate modules?

## Clean Code

- [x] Remove magic numbers in BouncingBallScene (radius, gravity, etc.)
- [ ] Error handling can certainly be improved everywhere, or not?
- [ ] Clarify and Refactor the include statements vs forward declarations

## Managers

- [ ] CameraManager, CameraSystem, Scrolling, Culling (?)
- [ ] Reusable ObjectPool
- [ ] We don't yet have any notion of AIController or NetworkController, TODO: Do IT!
- [x] ResourceManager
- [x] ServiceLocator
- [x] SceneManager
- [x] EventSystem
- [x] AudioManager
- [x] CommandRegistry, CommandSystem, Controllers
- [x] UIManager, GraphScene, SceneNode (this is handled by Flecs)

## Features

- [ ] SpatialPartitioning (grid)
- [ ] Load scenes, entities, components, systems from serialized files
- [x] CollisionSystem with collision shape (circle, rectangle)
  - [ ] Add other types of Collider (cf. Game Physics Engine Development)
- [x] SpriteRenderer
  - [ ] TileMap
  - [ ] SpriteAnimation
- [x] ControllerSystem (PlayerController, AIController, NetworkController)
  - [ ] Behaviors (State Machine, Behavior Tree)
  - [x] Possession
- [ ] AnimationSystem, SpriteAnimations
- [ ] ShaderEffects
- [x] ParticleSystem

## General Optimizations

### 1. Rendering Optimizations

- [ ] Batch rendering: Instead of drawing each entity individually, use `sf::VertexArray` or `sf::VertexBuffer` to draw multiple entities in a single draw call
- [ ] Frustum Culling: Only render what is visible on screen
- [ ] Sprite batching: Use texture atlases and batch similar sprites together
- [ ] Object pooling: Reuse entity objects instead of creating/destroying them

### 2. Resourcement Management

- [ ] Right now the assets have to be unique across all bundles, ideally, they should be accessible using BundleName::AssetName
- [ ] Probably need some kind of caching mechanism for assets so they load faster across scenes.

### 2. Physics Optimizations

- [ ] Spatial partitioning: Use a spatial hash grid, quadtree, or broad-phase collision detection

### 3. Flecs Optimizations

- [ ] ~~Create modules instead of individual scenes, each scene can be a module~~
- [ ] Have all the systems loaded at startup instead of loading them on demand. Systems should be data driven.
- [x] Have a single world handling everything so that the game state can be stored in it too

### 4. SFML-Specific Optimizations

- [ ] Reduce state changes: Minimize texture binding and shader changes.
- [ ] Use sf::Transform: Cache transformation matrices instead of recalculating them.
- [ ] Speaking of sf::Transform, we should cascade it down the entity hierarchy.
- [ ] ~~Profile with sf::Clock: Measure actual frame times to identify bottlenecks~~

### 5. General C++ Optimizations

- [ ] Avoid dynamic allocation: Use object pools and stack allocation where possible
- [ ] Compiler flags: Use `-O3` and appropriate optimization flags
- [x] Profile-guided optimization: Use profilers to identify hotspots

## Games

Some examples of games that can be implemented using my SFML-Flecs Engine:

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
    - [x] Pause/ simulation
    - [x] Reset scene

#### Physics Enhancements

- [x] Ball-to-Ball Collisions: Implement elastic collisions between balls
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
