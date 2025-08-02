// Copyright (c) Eric Jeker 2025.

#include "UIModule.h"

#include "Components/ButtonBackground.h"
#include "Components/ButtonText.h"
#include "Components/Clickable.h"
#include "Components/Hoverable.h"
#include "Components/Interactable.h"
#include "Modules/Render/Components/TextRenderable.h"
#include "Prefabs/Button.h"

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
