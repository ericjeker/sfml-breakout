// Copyright (c) Eric Jeker 2025.

#include "MiniRtsScene.h"

#include "../Components/MoveToOrderComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/SelectedComponent.h"
#include "../Components/TargetComponent.h"
#include "../Systems/DrawSelection.h"
#include "../Systems/MovementSystem.h"
#include "Components/DrawableComponent.h"
#include "Components/TransformComponent.h"
#include "Configuration.h"
#include "Systems/DrawableRenderer.h"
#include "Themes/Nord.h"
#include "Utilities/SFML.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>


MiniRtsScene::MiniRtsScene(ResourceManager& resourceManager, EventManager& eventManager)
    : Scene(resourceManager, eventManager)
{
}

void MiniRtsScene::Initialize()
{
    AddBackground();
    AddPlayerEntity();
    AddTargetEntity();

    // --- Add the Systems ---
    // TODO: CommandSystem (monitor entities with CommandQueueComponent)
    // TODO: MovementSystem (MoveToOrderComponent)
    AddSystem(std::make_unique<MovementSystem>());
    // TODO: AttackSystem (AttackOrderComponent)
    // TODO: AIControllerSystem (monitor entities with AIControlledComponent, state, perception, issue orders)
    // TODO: CollisionSystem & PathingSystem
    // TODO: AnimationSystem (monitor AnimationStateComponent)
    // TODO: ParticleEmitterSystem (ParticleRequestComponent, create entities with ParticleEmitterComponent)
    // TODO: ParticleUpdateSystem (monitor ParticleEmitterComponent)
    // TODO: DrawSelectionSystem (SelectedComponent)
    AddSystem(std::make_unique<DrawableRenderer>());
    AddSystem(std::make_unique<DrawSelection>());
}

void MiniRtsScene::HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    if (!IsLoaded() || IsPaused())
    {
        return;
    }

    // TODO: Escape or Click in empty space deselect units

    // TODO: Click on a selectable entity select it

    // TODO: Click + Drag select multiple selectable units

    if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
    {
        const sf::Vector2f position{mousePressed->position};

        if (mousePressed->button == sf::Mouse::Button::Right)
        {
            // TODO: If LShift or RShift is pressed, queue orders
            // TODO: Check if units are selected, identify them, add a MoveToOrder component to them
            if (auto* player = GetEntityWithComponent<SelectedComponent>())
            {
                LOG_DEBUG("(MiniRtsScene:HandleEvent): Add MoveToOrder");
                player->AddComponent<MoveToOrderComponent>({.target = position});
            }

            // --- Draw the Target ---
            auto* entity = GetEntityWithComponent<TargetComponent>();
            if (!entity)
            {
                return;
            }

            auto* drawable = entity->GetComponent<DrawableComponent>();
            drawable->isVisible = true;
            auto* transform = entity->GetComponent<TransformComponent>();
            transform->position = position;
        }
        else if (mousePressed->button == sf::Mouse::Button::Left)
        {
            // Select the clicked entity and mark it as controlled by the player
            for (const auto& entity : GetEntities())
            {
                if (entity->HasComponent<DrawableComponent>() && entity->HasComponent<TransformComponent>())
                {
                    if (const auto* drawable = entity->GetComponent<DrawableComponent>();
                        GetDrawableBounds(drawable).contains(position))
                    {
                        entity->AddComponent<SelectedComponent>({});
                    }
                    else
                    {
                        entity->RemoveComponent<SelectedComponent>();
                    }
                }
            }
        }
    }
}

void MiniRtsScene::AddBackground()
{
    auto rect = std::make_unique<sf::RectangleShape>();
    rect->setSize(sf::Vector2f{Configuration::WINDOW_SIZE});
    rect->setFillColor(NordTheme::Frost1);

    auto entity = std::make_unique<Entity>(GenerateId());
    entity->AddComponent<DrawableComponent>({.drawable = std::move(rect)});
    entity->AddComponent<TransformComponent>({});
    AddEntity(std::move(entity));
}

void MiniRtsScene::AddPlayerEntity()
{
    auto shape = std::make_unique<sf::CircleShape>(10.f);
    shape->setFillColor(NordTheme::Aurora1);

    auto entity = std::make_unique<Entity>(GenerateId());
    entity->AddComponent<TransformComponent>({.position = {100.f, 100.f}});
    entity->AddComponent<DrawableComponent>({.type = DrawableType::CircleShape, .drawable = std::move(shape)});
    entity->AddComponent<MovementComponent>({});
    AddEntity(std::move(entity));
}

void MiniRtsScene::AddTargetEntity()
{
    auto shape = std::make_unique<sf::RectangleShape>(sf::Vector2f{5.f, 5.f});
    shape->setFillColor(NordTheme::PolarNight2);

    auto entity = std::make_unique<Entity>(GenerateId());
    entity->AddComponent<DrawableComponent>({.drawable = std::move(shape), .isVisible = false});
    entity->AddComponent<TransformComponent>({});
    entity->AddComponent<TargetComponent>({});
    AddEntity(std::move(entity));
}
