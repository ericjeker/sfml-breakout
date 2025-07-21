// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include "../Components/BoundingBoxComponent.h"
#include "Systems/System.h"


class EventSystem final : public System
{
public:
    void HandleEvent(const std::unique_ptr<Entity>& entity, const std::optional<sf::Event>& event, sf::RenderWindow& window) override;

private:
    static sf::FloatRect GetBoundingBoxFromDrawable(const std::unique_ptr<Entity>& entity, BoundingBoxComponent* boundingBoxComponent);
};


#endif
