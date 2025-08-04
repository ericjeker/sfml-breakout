// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/UI/UIModule.h"

#include "Core/Modules/Render/Components/TextRenderable.h"
#include "Core/Modules/UI/Components/ButtonBackground.h"
#include "Core/Modules/UI/Components/ButtonText.h"
#include "Core/Modules/UI/Components/Clickable.h"
#include "Core/Modules/UI/Components/Hoverable.h"
#include "Core/Modules/UI/Components/Interactable.h"
#include "Core/Modules/UI/Prefabs/Button.h"

namespace Modules
{

UIModule::UIModule(const flecs::world& world)
{
    // --- Declare Components ---
    world.component<ButtonBackground>();
    world.component<ButtonText>();
    world.component<Clickable>();
    world.component<Hoverable>();
    world.component<Interactable>();
    world.component<TextRenderable>();
}

} // namespace Modules
