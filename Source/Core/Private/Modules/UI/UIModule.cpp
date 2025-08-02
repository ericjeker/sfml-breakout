// Copyright (c) Eric Jeker 2025.

#include "Modules/UI/UIModule.h"

#include "Modules/Render/Components/TextRenderable.h"
#include "Modules/UI/Components/ButtonBackground.h"
#include "Modules/UI/Components/ButtonText.h"
#include "Modules/UI/Components/Clickable.h"
#include "Modules/UI/Components/Hoverable.h"
#include "Modules/UI/Components/Interactable.h"
#include "Modules/UI/Prefabs/Button.h"

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
