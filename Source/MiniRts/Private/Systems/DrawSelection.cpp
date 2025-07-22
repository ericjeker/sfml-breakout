// Copyright (c) Eric Jeker 2025.

#include "DrawSelection.h"

#include "../Components/SelectedComponent.h"
#include "Components/DrawableComponent.h"
#include "Utilities/SFML.h"

#include <SFML/Graphics/CircleShape.hpp>


void DrawSelection::Render(const std::unique_ptr<Entity>& entity, sf::RenderWindow& window)
{
    if (!entity->HasComponent<SelectedComponent>())
    {
        return;
    }

    // TODO: Use an object pool
    const auto* drawable = entity->GetComponent<DrawableComponent>();
    const auto bounds = GetDrawableBounds(drawable);
    auto selectionCircle = sf::RectangleShape{{bounds.size.x + 2.f, bounds.size.y + 2.f}};
    selectionCircle.setFillColor(sf::Color::Transparent);
    selectionCircle.setPosition({bounds.position.x - 1.f, bounds.position.y - 1.f});
    selectionCircle.setOutlineColor(sf::Color::Black);
    selectionCircle.setOutlineThickness(2.f);
    window.draw(selectionCircle);
}
