set(SOURCE_FILES
        Private/Scenes/MiniRtsScene.cpp
)

add_library(MiniRts)
target_sources(MiniRts PRIVATE ${SOURCE_FILES})
target_include_directories(MiniRts PUBLIC Public)
target_link_libraries(MiniRts PRIVATE Vendor Core)
