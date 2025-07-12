# SFML Sandbox: Animations

A new SFML Sandbox to try different architectures for a game engine.

## Call Hierarchy

```
Application -> GameController -> GameState -> SceneManager -> Scene -> Systems -> Components (EnTT)
```

## Application / Game Instance

The responsibility of the game instance is to load the base resources. This class owns the different
managers stored in the engine context.

It is also where the game loop is started, and the root of the call hierarchy.

The Game Instance holds the instance of the Window where everything will be drawn.

Four events are processed:

- Closed: when the user closes the window (Alt+F4, or clicking on the close button)
- Resized: the user resized the window, this event is pushed down the call hierarchy so that the scene can be redrawn
- FocusLost: pause the game loop
- FocusGained: resume the game loop

## GameController

Responsible for handling the game flow using a Stack State Pattern holding different GameStates. Multiple game states can
be active at the same time using a push/pop system.

The GameController also manages the global game state that persists across all GameStates.

## GameState

Hold the current game mode, encapsulate state-specific behavior and data. The GameState also manage which scene should
be active given its particular state.

The GameState to Scene relationship can be:

- One-One State to Scene
- One State instantiated with Dynamic Scene
- One-Many State to Scenes

## SceneManager

Responsible for loading scenes and keeping track of the current loaded scene. The SceneManager can
overlay multiple scenes, or load scenes additively.

## Scene / UIScene

A scene is a cohesive set of game objects (entities) or UI Elements that can be handled with an ECS system (EnTT) or
a UI Manager.

