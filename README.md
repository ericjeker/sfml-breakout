# SFML Sandbox: Animations

A new SFML Sandbox to try different architectures for a game engine.

## Call Hierarchy

```
GameInstance -> GameController -> GameState
                               -> SceneManager -> Scene -> Systems
                                                        -> Entities -> Components
```

## ECS-Lite

The Scene holds entities in an ECS-lite architecture. Entities (`Entity`) contain Components. Scene contains Systems.
But the "Lite" comes from the fact that there is no query system. The systems always receive all the entities and
they can check for each of them if they have a given component.

## Game Instance

The responsibility of the game instance is to load the base resources. This class owns the different
managers stored in the `GameService` service locator class.

It is also where the game loop is started (`Run()`), and the root of the call hierarchy.

The `GameInstance` also holds the instance of the Window where everything will be drawn.

## GameController

Responsible for handling the game flow using a Stack State Pattern holding different GameStates. Multiple game states can
be active at the same time using a push/pop system.

The GameController also manages the global game state that persists across all GameStates.

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

## Scene / ECSScene / UIScene / CinematicScene

A scene is a cohesive set of game objects (entities) or UI Elements that can be handled with an ECS system or
a UI Manager.

