// Copyright (c) Eric Jeker 2025.

#include "BouncingBallScene.h"

#include "ApplicationConfiguration.h"
#include "Components/DrawableComponent.h"
#include "Components/PhysicsComponent.h"
#include "Components/TransformComponent.h"
#include "Systems/DrawableRenderer.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/ScreenBounceSystem.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include <Themes/Nord.h>
#include <random>


BouncingBallScene::BouncingBallScene(ResourceManager& resourceManager, EventManager& eventManager)
    : Scene(resourceManager, eventManager)
{
}

void BouncingBallScene::Initialize()
{
    // Create a background
    auto background = std::make_unique<sf::RectangleShape>();
    background->setSize(sf::Vector2f{ApplicationConfiguration::windowSize});
    background->setFillColor(NordTheme::Frost1);

    auto backgroundEntity = std::make_unique<Entity>(GenerateId());
    backgroundEntity->AddComponent<DrawableComponent>({.drawable = std::move(background)});
    backgroundEntity->AddComponent<TransformComponent>({});

    AddEntity(std::move(backgroundEntity));

    // Create balls
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(0.f, ApplicationConfiguration::windowSize.x);
    std::uniform_real_distribution<float> distY(0.f, ApplicationConfiguration::windowSize.y);
    std::uniform_real_distribution<float> velX(0.f, 100.f);
    std::uniform_real_distribution<float> velY(0.f, 100.f);

    // For now, I can go up to 5000 at 60 FPS, sounds pretty poor...
    // But let's show some self-compassion and be happy that the system works.
    for (int i = 0; i < 50; ++i)
    {
        // Create a ball
        auto ball = std::make_unique<sf::CircleShape>(10.f);
        ball->setFillColor(NordTheme::Aurora1);
        ball->setOrigin({5.f, 5.f});

        const sf::Vector2f randomPosition = {distX(gen), distY(gen)};
        const sf::Vector2f velocity = {velX(gen), velY(gen)};

        auto ballEntity = std::make_unique<Entity>(GenerateId());
        ballEntity->AddComponent<DrawableComponent>({.drawable = std::move(ball)});
        ballEntity->AddComponent<TransformComponent>({.position = randomPosition});
        ballEntity->AddComponent<PhysicsComponent>({.velocity = velocity, .friction = 0.98});

        AddEntity(std::move(ballEntity));
    }

    // Add the systems
    constexpr float pixelsPerMeter = ApplicationConfiguration::windowSize.x / 2.f;
    AddSystem(std::make_unique<PhysicsSystem>(sf::Vector2f{0.f, 9.81f}, pixelsPerMeter));
    AddSystem(std::make_unique<ScreenBounceSystem>(ApplicationConfiguration::windowSize));
    AddSystem(std::make_unique<DrawableRenderer>());
}

void BouncingBallScene::HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    Scene::HandleEvent(event, window);

    if (!IsLoaded())
    {
        return;
    }

    if (event->is<sf::Event::MouseButtonPressed>())
    {
        LOG_DEBUG("(BouncingBallScene:HandleEvent): Mouse button pressed");
    }
}
