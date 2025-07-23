// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef BOUNCINGBALLFLECSSCENE_H
#define BOUNCINGBALLFLECSSCENE_H

#include "Scenes/Scene.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <flecs.h>

struct Transform
{
    sf::Vector2f position = {0.f, 0.f};
    sf::Vector2f scale = {1.f, 1.f};
    float rotation = 0.f;
};

struct Velocity
{
    sf::Vector2f velocity = {0.f, 0.f};
};

struct RigidBody {
    float damping = 1.f;
    float restitution;
    bool isKinematic;
};

struct Collider {
    float radius;
    uint32_t layers;
    bool isTrigger;
};

struct BallRenderable
{
    std::unique_ptr<sf::CircleShape> shape;
};

struct BackgroundRenderable
{
    std::unique_ptr<sf::RectangleShape> shape;
};

struct GravitySettings {
    sf::Vector2f gravity;
    float pixelsPerCentimeter;
    bool enabled = true;
};

class BouncingBallFlecsScene final : public Scene
{
public:
    BouncingBallFlecsScene(ResourceManager& resourceManager, EventManager& eventManager);

    void Initialize() override;
    void HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
    void CreateBalls(int count);

    flecs::world& GetWorld();

private:
    static void ProcessScreenBounce(const flecs::iter& it, size_t, Transform& t, Velocity& v);
    static void ProcessPhysics(const flecs::iter& it, size_t, Transform& t, Velocity& v, const RigidBody& p);

    // Flecs entities for singletons
    flecs::world _world;
};


#endif //BOUNCINGBALLFLECSSCENE_H
