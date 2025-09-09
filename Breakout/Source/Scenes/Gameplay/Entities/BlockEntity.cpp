// Copyright (c) Eric Jeker 2025.

#include "BlockEntity.h"

#include "Scenes/Gameplay/Components/Block.h"
#include "Scenes/Gameplay/Components/Health.h"
#include "Scenes/Gameplay/Components/Indestructible.h"

#include "Core/Modules/Physics/Components/ColliderShape.h"
#include "Core/Modules/Render/Factories/Rectangle.h"


flecs::entity BlockEntity::Create(const flecs::world& world, const flecs::entity& rootEntity, const BlockParams& params)
{
    const auto
        entity = Factories::Rectangle::
                     Create(world, {.size = params.size, .color = params.color, .origin = {0.f, 0.f}, .position = params.position})
                         .add<Block>()
                         .set<Health>({params.health})
                         .set<ColliderShape>({Shape::Rectangle});

    if (params.isIndestructible)
    {
        entity.add<Indestructible>();
    }

    entity.child_of(rootEntity);

    return entity;
}
