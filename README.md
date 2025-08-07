# SFML Sandbox: Flecs World

A new SFML Sandbox to try different architectures for a game engine. This time, I integrate with Flecs. The game instance
creates a unique Flecs world passed down to the scenes.

## Call Hierarchy

```
GameInstance -> Flecs World (Systems, Singletons) -> GameStateManager -> GameState
                                                  -> SceneManager -> Scene -> Systems
                                                                           -> Entities -> Components
```

## Game Instance

The responsibility of the game instance is to load the base resources. This class also instanciates the Flecs World later
passed down to the scenes.

It is also where the game loop is started (`Run()`), and the root of the call hierarchy. The game loop makes the world
progress by calling the `progress()` function.

## GameStateManager

Responsible for handling the game flow using a Stack State Pattern holding different GameStates. Multiple game states can
be active at the same time using a push/pop system.

## GameState

Hold the current game mode, encapsulate state-specific behavior and data. The GameState also manages which scene should
be active given its particular state.

The GameState to Scene relationship can be:

- One-One State to Scene
- One State instantiated with Dynamic Scene
- One-Many State to Scenes

## SceneManager

Responsible for loading and unloading scenes and keeping track of the current loaded scene. The SceneManager can load
scenes in two different modes: single or additive. `Single` mode cleans up the previously loaded scenes before loading
the new scene. `Additive` loads the new scene on top of the previous one as to create an overlay system.

Scenes are initialized in the `SceneManager::LoadScene()` function.

## Scene

A scene is a cohesive set of game objects (entities) add to the Flecs world. Each scene has its own root entity to which
all other entities are added. When a scene is unloaded, the root entity is destroyed along with all its children.

## Copyrights & Credits

Music by Karl Casey @ [White Bat Audio](https://www.youtube.com/@WhiteBatAudio) -- Check his music on [YouTube](https://www.youtube.com/@WhiteBatAudio)

