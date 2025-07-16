set(SOURCE_FILES
        Private/Animations.cpp
        Private/Main.cpp
        Private/Gameplay/MainMenuState.cpp
        Private/Scenes/DemoScene.cpp
        Private/Scenes/PauseScene.cpp
)

# --- Create the main library ---
add_library(AnimationsLibrary)
target_sources(AnimationsLibrary PRIVATE ${SOURCE_FILES})
target_include_directories(AnimationsLibrary PUBLIC Public)
target_include_directories(AnimationsLibrary PRIVATE Private)
target_link_libraries(AnimationsLibrary PRIVATE Vendor Core)

# --- Create the main executable ---
add_executable(Animations)
target_sources(Animations PRIVATE ${SOURCE_FILES})
target_include_directories(AnimationsLibrary PRIVATE Private)
target_link_libraries(Animations PRIVATE Vendor Core AnimationsLibrary)
# Enable debug mode if we are in the debug build
target_compile_definitions(Animations PRIVATE $<$<CONFIG:Debug>:DEBUG>)
