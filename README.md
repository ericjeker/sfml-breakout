# SFML Breakout

This repository started as a SFML sandbox project to learn how to use SFML and Flecs, later I decided to create a
breakout game to test the engine.

## Call Hierarchy

```
GameInstance -> Flecs World (Systems, Singletons) -> GameStateManager -> GameState
                -> Global Systems                    -> SceneManager -> Scene -> Local Systems
                -> Singletons                                                 -> Entities -> Components
```

## GameInstance

The responsibility of the game instance is to load the base resources. This class also instantiates the Flecs World later
passed down to the scenes.

It is also where the game loop is started (`GameInstance::Run()`), and the root of the call hierarchy. The game loop
makes the world progress by calling the `progress()` function.

## GameStateManager

Responsible for handling the game flow using a Stack State Pattern holding different GameStates. Multiple game states can
be active at the same time using a push/pop system.

## GameState

Hold the current game state, or game mode, encapsulate state-specific behavior and data. The GameState also manages what
scene should be active given its particular state. There can only be one state active at a time, but multiple scenes can
be active at the same time.

The GameState to Scene relationship can be:

- One-One State to Scene
- One State instantiated with Dynamic Scene
- One-Many State to Scenes

## SceneManager

Responsible for loading and unloading scenes and keeping track of the current loaded scene. The SceneManager can load
scenes in two different modes: single or additive. `Single` mode cleans up the previously loaded scenes before loading
the new scene. `Additive` loads the new scene on top of the previous one as to create an overlay system.

Scenes are initialized in the `SceneManager::LoadScene()` function and shutdown in the `SceneManager::UnloadScene()`
function.

## Scene

A scene is a cohesive set of game objects (entities) added to the Flecs world. Each scene has its own root entity to
which all other entities are children of. When a scene is unloaded, the root entity is destroyed along with all its
children.

On initialization, they will instantiate entities and register scene-local systems when necessary. The scene-local
systems will be unregistered when the scene is unloaded. They operate only on the local entities.

A scene should hold no state. All states should be stored in the Flecs world as Singletons or Entities.
A scene should be serializable to allow for a future save/load mechanism.

## Flecs Architecture

### Modules

Modules are the main components of the game. They are responsible for loading and unloading the resources needed by the
game. They are global and are not tied to a specific scene.

These modules are imported during initialization of the game instance.

Examples: Control, Physics, Render, UI, ...

## World Progress

The world progress is the main game loop. It is called by the game instance and progress the world by calling the
`progress()` function.

Before the world progresses, we will handle the input event from SFML. This is done using a polling mechanism adapted to
UI elements interaction or single key presses.

### Phases

System phases are called in the following order:

#### flecs::OnStart

#### flecs::OnLoad

#### flecs::PostLoad

* Add UI events taken from the events pool
* Add control commands from InputBindings
* Process UI events & Control commands
  * Hit Test on UI Elements

#### flecs::PreUpdate

#### flecs::OnUpdate

* Physics
* Animation
* Movement
* Collision

#### flecs::OnValidate

#### flecs::PostUpdate

* Destroy entities that are no longer needed (e.g. LifetimeOneFrame)

#### flecs::PreStore

* Apply transformations to entities

#### flecs::OnStore

* Render

#### flecs::PostFrame

* End of frame events

## Copyrights & Credits

Music by Karl Casey @ [White Bat Audio](https://www.youtube.com/@WhiteBatAudio) -- Check his music on [YouTube](https://www.youtube.com/@WhiteBatAudio)

