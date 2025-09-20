# --- Include the FetchConccc
include(FetchContent)

# --- Tracy Profiler ---
FetchContent_Declare(tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG v0.12.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_MakeAvailable(tracy)

# --- SFML ---
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

# --- JSON Processing ---
FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_MakeAvailable(json)

# --- Flecs ECS ---
FetchContent_Declare(flecs
        GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
        GIT_TAG v4.1.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_MakeAvailable(flecs)

# --- Library creation and linking ---
add_library(Vendor INTERFACE)
target_link_libraries(Vendor INTERFACE
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
        nlohmann_json::nlohmann_json
        Tracy::TracyClient
        flecs::flecs_static
)
