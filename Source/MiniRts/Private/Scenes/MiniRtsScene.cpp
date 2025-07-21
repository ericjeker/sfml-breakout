// Copyright (c) Eric Jeker 2025.

#include "MiniRtsScene.h"

#include "../Components/TargetComponent.h"
#include "Components/DrawableComponent.h"
#include "Components/TransformComponent.h"
#include "Configuration.h"
#include "Systems/DrawableRenderer.h"
#include "Themes/Nord.h"

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

    AddSystem(std::make_unique<DrawableRenderer>());
}
void MiniRtsScene::HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    if (!IsLoaded() || IsPaused())
    {
        return;
    }

    if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
    {
        LOG_DEBUG("(MiniRtsScene:HandleEvent): Mouse button pressed");
        if (mousePressed->button == sf::Mouse::Button::Right)
        {
            auto* entity = GetEntityWithComponent<TargetComponent>();
            if (!entity)
            {
                return;
            }

            auto* drawable = entity->GetComponent<DrawableComponent>();
            drawable->isVisible = true;
            auto* transform = entity->GetComponent<TransformComponent>();
            transform->position = sf::Vector2f{mousePressed->position};
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
    entity->AddComponent<DrawableComponent>({.drawable = std::move(shape)});
    AddEntity(std::move(entity));
}

void MiniRtsScene::AddTargetEntity()
{
    auto shape = std::make_unique<sf::RectangleShape>(sf::Vector2f{5.f, 5.f});
    shape->setFillColor(NordTheme::PolarNight1);

    auto entity = std::make_unique<Entity>(GenerateId());
    entity->AddComponent<DrawableComponent>({.drawable = std::move(shape), .isVisible = false});
    entity->AddComponent<TransformComponent>({});
    entity->AddComponent<TargetComponent>({});
    AddEntity(std::move(entity));
}
