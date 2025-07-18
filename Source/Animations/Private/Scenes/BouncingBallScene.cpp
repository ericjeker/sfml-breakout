// Copyright (c) Eric Jeker 2025.

#include "BouncingBallScene.h"

#include "../Components/PlayerPossessedComponent.h"
#include "../Systems/CleanUpSystem.h"
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
    std::uniform_real_distribution<float> velX(-100.f, 100.f);
    std::uniform_real_distribution<float> velY(-100.f, 100.f);

    // For now, I can go up to 8000 at 60 FPS, sounds pretty poor...
    // But let's show some self-compassion and be happy that the system works.
    //
    // #1: first optimization is to not be dumb and repeat the entity loop in every system... lol...
    //
    // The reason it's slow is that I slap the components to the entities (it's an ECS-lite) and end up with a standard
    // array of structure (entities) instead of a structure of an array with column orientation. To re-orient my data,
    // I should have an array for each component type: Transform, Physics, etc. Right now I am using
    // the composition pattern, not an ECS architecture.
    for (int i = 0; i < 100; ++i)
    {
        const sf::Vector2f position = {distX(gen), distY(gen)};
        const sf::Vector2f velocity = {velX(gen), velY(gen)};
        AddEntity(std::move(CreateBallEntity(position, velocity)));
    }

    // --- Add the systems ---
    constexpr float pixelsPerCentimeters = ApplicationConfiguration::windowSize.x / 200.f;
    AddSystem(std::make_unique<PhysicsSystem>(sf::Vector2f{0.f, 0.f}, pixelsPerCentimeters));
    AddSystem(std::make_unique<ScreenBounceSystem>(ApplicationConfiguration::windowSize));
    AddSystem(std::make_unique<CleanUpSystem>());
    AddSystem(std::make_unique<DrawableRenderer>());
}

void BouncingBallScene::HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    Scene::HandleEvent(event, window);

    if (!IsLoaded())
    {
        return;
    }

    // --- Handle the mouse events ---
    if (event->is<sf::Event::MouseButtonPressed>())
    {
        // Create a ball and glue it to the mouse cursor
        _throwStartPos = sf::Mouse::getPosition(window);
        auto entity = CreateBallEntity(sf::Vector2f(_throwStartPos), sf::Vector2f{0.f, 0.f});
        entity->AddComponent<PlayerPossessedComponent>({});
        AddEntity(std::move(entity));
    }
    else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
    {
        // Update the mouse cursor position
        const auto position = mouseMoved->position;
        if (auto* player = GetEntityWithComponent<PlayerPossessedComponent>(); player != nullptr)
        {
            auto* transform = player->GetComponent<TransformComponent>();
            transform->position = sf::Vector2f(position);
        }
    } else if (event->is<sf::Event::MouseButtonReleased>())
    {
        const auto position = sf::Mouse::getPosition(window);
        const auto delta = position - _throwStartPos;
        LOG_DEBUG(
            std::format(
                "(BouncingBallScene:HandleEvent): Mouse button release with force {}",
                static_cast<sf::Vector2f>(delta).length()
            )
        );
        if (auto* player = GetEntityWithComponent<PlayerPossessedComponent>(); player != nullptr)
        {
            constexpr float pixelsPerCentimeters = ApplicationConfiguration::windowSize.x / 200.f;
            player->RemoveComponent<PlayerPossessedComponent>();
            auto* transform = player->GetComponent<TransformComponent>();
            transform->position = sf::Vector2f(position);
            auto* physics = player->GetComponent<PhysicsComponent>();
            physics->velocity = sf::Vector2f(delta.x, delta.y) * pixelsPerCentimeters;
        }
    }
}

std::unique_ptr<Entity> BouncingBallScene::CreateBallEntity(const sf::Vector2f position, const sf::Vector2f velocity)
{
    constexpr float radius = 5.f;

    // Create a ball
    auto ball = std::make_unique<sf::CircleShape>(radius);
    ball->setFillColor(NordTheme::Aurora1);
    ball->setOrigin({radius, radius});

    auto ballEntity = std::make_unique<Entity>(GenerateId());
    ballEntity->AddComponent<DrawableComponent>({.drawable = std::move(ball)});
    ballEntity->AddComponent<TransformComponent>({.position = position});
    ballEntity->AddComponent<PhysicsComponent>({.velocity = velocity, .friction = 1.f});

    return ballEntity;
}
