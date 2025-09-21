// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/Event/EventModule.h"

#include "Core/Modules/Event/Components/EventBindings.h"
#include "Core/Modules/Input/Components/Command.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/Scene/Components/SceneDepth.h"
#include "Core/Modules/Scene/Tags/ScenePaused.h"
#include "Core/Modules/Scene/Tags/SceneRoot.h"
#include "Core/Modules/UI/Components/KeyPressed.h"
#include "Core/Utils/Logger.h"

#include <algorithm>
#include <ranges>

namespace
{

auto ProcessKeyPressedEvents()
{
    return [](const flecs::entity& e, const KeyPressed& k) {
        // Generate the event key
        const auto eventKey = InputKey::Keyboard(k.code);

        const auto sceneQuery = e.world()
                                    .query_builder<const SceneDepth, EventBindings>()
                                    .with<SceneRoot>()
                                    .without<ScenePaused>()
                                    .build();

        // Collect entities and sort manually
        std::vector<std::pair<flecs::entity, SceneDepth>> entities;
        sceneQuery.each([&entities](const flecs::entity& rootEntity, const SceneDepth& depth, EventBindings&) {
            entities.emplace_back(rootEntity, depth);
        });

        // Sort by depth
        std::ranges::sort(entities, [](const auto& a, const auto& b) { return a.second.depth > b.second.depth; });

        // Process sorted entities
        for (const auto& rootEntity : entities | std::views::keys)
        {
            auto [map] = rootEntity.get<EventBindings>();
            if (!map.contains(eventKey))
            {
                return;
            }

            LOG_DEBUG("EventModule::ProcessKeyPressedEvents -> Processing event for scene {} -> {}", std::string(rootEntity.name()), rootEntity.id());
            rootEntity.world().entity().is_a(map[eventKey]);
            break;
        }
    };
}

} // namespace


namespace Core::Modules
{

EventModule::EventModule(const flecs::world& world)
{
    world.component<EventBindings>();

    world.system<const KeyPressed>("EventModule::ProcessKeyPressedEvents").kind(flecs::PostLoad).each(ProcessKeyPressedEvents());
}

} // namespace Core::Modules
