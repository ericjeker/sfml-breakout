set(SOURCE_FILES
        Private/Logger.cpp
        Private/GameInstance.cpp
        Private/Gameplay/GameController.cpp
        Private/Gameplay/GameState.cpp
        Private/Managers/GameService.cpp
        Private/Managers/FileManager.cpp
        Private/Managers/ResourceManager.cpp
        Private/Managers/SceneManager.cpp
        Private/Scenes/Entity.cpp
        Private/Scenes/Scene.cpp
        Private/Systems/System.cpp
)

add_library(Core)
target_sources(Core PRIVATE ${SOURCE_FILES})
target_include_directories(Core PUBLIC Public)
target_include_directories(Core PRIVATE Private)
target_link_libraries(Core PRIVATE Vendor)

# Enable debug mode if we are in the debug build
target_compile_definitions(Core PRIVATE $<$<CONFIG:Debug>:DEBUG>)
