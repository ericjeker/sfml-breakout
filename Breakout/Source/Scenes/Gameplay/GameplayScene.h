// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Core/Scenes/Scene.h"

#include <map>


class GameplayScene final : public Scene
{
public:
    explicit GameplayScene(flecs::world& world);
    void Initialize() override;

private:
    struct BlockDefinition
    {
        sf::Color color = sf::Color::White;
        int health{};
        bool isIndestructible{};
    };

    void CreateInputBindings(const flecs::world& world);
    static std::map<char, BlockDefinition> LoadBlockDefinitions(const std::string& filename);
    void CreateBlocks(const flecs::world& world, float& zOrder);
    void CreateBackground(const flecs::world& world, float& zOrder);
};



