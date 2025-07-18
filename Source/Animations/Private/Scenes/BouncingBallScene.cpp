// Copyright (c) Eric Jeker 2025.

#include "BouncingBallScene.h"

#include "../Components/PlayerPossessedComponent.h"
#include "../Events/BallCountChangedEvent.h"
#include "../Events/RequestBallCountEvent.h"
#include "../Systems/CleanUpSystem.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include <ApplicationConfiguration.h>
#include <PhysicsConstants.h>
#include <Systems/DrawableRenderer.h>
#include <Systems/PhysicsSystem.h>
#include <Systems/ScreenBounceSystem.h>
#include <Themes/Nord.h>
#include <random>

#include <Components/DrawableComponent.h>
#include <Components/PhysicsComponent.h>
#include <Components/TransformComponent.h>

BouncingBallScene::BouncingBallScene(ResourceManager& resourceManager, EventManager& eventManager)
    : Scene(resourceManager, eventManager)
{
}

void BouncingBallScene::Initialize()
{
    // Create a background
    auto background = std::make_unique<sf::RectangleShape>();
    background->setSize(sf::Vector2f{ApplicationConfiguration::WINDOW_SIZE});
    background->setFillColor(NordTheme::Frost1);

    auto backgroundEntity = std::make_unique<Entity>(GenerateId());
    backgroundEntity->AddComponent<DrawableComponent>({.drawable = std::move(background)});
    backgroundEntity->AddComponent<TransformComponent>({});

    AddEntity(std::move(backgroundEntity));

    CreateBalls(100);

    // --- Add the systems ---
    AddSystem(std::make_unique<PhysicsSystem>(PhysicsConstants::GRAVITY_DOWN, PhysicsConstants::PIXELS_PER_CENTIMETER));
    AddSystem(std::make_unique<ScreenBounceSystem>(ApplicationConfiguration::WINDOW_SIZE));
    AddSystem(std::make_unique<CleanUpSystem>());
    AddSystem(std::make_unique<DrawableRenderer>());

    // --- Add event listeners ---
    GetEventManager().Subscribe<RequestBallCountEvent>(
        [this](const RequestBallCountEvent& event, void* sender)
        { GetEventManager().Emit<BallCountChangedEvent>({.count = static_cast<int>(GetEntities().size() - 1)}, this); }
    );
}

void BouncingBallScene::HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    Scene::HandleEvent(event, window);

    if (!IsLoaded() || IsPaused())
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
        // Emit an event to notify the UI that the ball count has changed.
        GetEventManager().Emit<BallCountChangedEvent>({.count = static_cast<int>(GetEntities().size() - 1)}, this);
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
    }
    else if (event->is<sf::Event::MouseButtonReleased>())
    {
        // When we release the mouse button, we calculate the distance with the initial button press, the resulting
        // delta is the velocity given to the ball
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
            player->RemoveComponent<PlayerPossessedComponent>();
            player->AddComponent<PhysicsComponent>(
                {.velocity = sf::Vector2f(delta.x, delta.y) * PhysicsConstants::PIXELS_PER_CENTIMETER,
                 .damping = PhysicsConstants::DEFAULT_DAMPING}
            );

            if (auto* transform = player->GetComponent<TransformComponent>())
            {
                transform->position = sf::Vector2f(position);
            }
        }
    }
    // --- Handle the keyboard events ---
    else if (auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scan::G)
        {
            ToggleGravity();
        }
        else if (keyPressed->scancode == sf::Keyboard::Scan::Backspace || keyPressed->scancode == sf::Keyboard::Scan::R)
        {
            ResetSimulation();
        }
        else if (keyPressed->scancode == sf::Keyboard::Scan::Up)
        {
            IncreaseGravity();
        }
        else if (keyPressed->scancode == sf::Keyboard::Scan::Down)
        {
            DecreaseGravity();
        }
        else if (keyPressed->scancode == sf::Keyboard::Scan::NumpadPlus)
        {
            int numberOfBalls = 1;
            if (isKeyPressed(sf::Keyboard::Scan::LShift) || isKeyPressed(sf::Keyboard::Scan::RShift))
            {
                numberOfBalls *= 10;
            }

            CreateBalls(numberOfBalls);
        }
    }
}
void BouncingBallScene::DecreaseGravity()
{
}

void BouncingBallScene::IncreaseGravity()
{
}

void BouncingBallScene::ResetSimulation()
{
    ClearEntitiesWithComponent<PhysicsComponent>();
    CreateBalls(100);
}

void BouncingBallScene::ToggleGravity()
{
    auto* system = GetSystem<PhysicsSystem>();
    system->ToggleGravity();
}

void BouncingBallScene::CreateBalls(const int numberOfBalls = 100)
{
    // Create balls
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(0.f, ApplicationConfiguration::WINDOW_SIZE.x);
    std::uniform_real_distribution<float> distY(0.f, ApplicationConfiguration::WINDOW_SIZE.y);
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
    for (int i = 0; i < numberOfBalls; ++i)
    {
        const sf::Vector2f position = {distX(gen), distY(gen)};
        const sf::Vector2f velocity =
            {velX(gen) * PhysicsConstants::PIXELS_PER_CENTIMETER, velY(gen) * PhysicsConstants::PIXELS_PER_CENTIMETER};
        AddEntity(std::move(CreateBallEntity(position, velocity)));
        // Emit an event to notify the UI that the ball count has changed.
        GetEventManager().Emit<BallCountChangedEvent>({.count = static_cast<int>(GetEntities().size() - 1)}, this);
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
    ballEntity->AddComponent<PhysicsComponent>({.velocity = velocity, .damping = PhysicsConstants::DEFAULT_DAMPING});

    return ballEntity;
}
