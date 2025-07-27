// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "Components/Transform.h"
#include "Modules/Render/Components/RectangleShape.h"
#include "Modules/Render/Components/TextRenderable.h"
#include "Scenes/Scene.h"

#include <SFML/Graphics/Text.hpp>

class MainMenuScene final : public Scene
{
public:
    void Initialize() override;
    void HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;

private:
    void CreateTextEntity(std::unique_ptr<sf::Text> text, sf::Vector2f position);
    void CreateButtonEntity(std::unique_ptr<sf::Text> text, sf::Vector2f position, const std::function<void()>& callback);
    static void ProcessText(const Transform& t, const TextRenderable& textRenderable);
    static void ProcessBackground(const Transform& t, const RectangleShape& bg);
};


#endif
