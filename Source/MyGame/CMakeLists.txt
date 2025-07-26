# --- Options and Configurations ---
option(TRACY_ENABLE "Enable Tracy profiler" ON)
option(TRACY_VERBOSE "Enable Tracy verbose output" ON)

# --- Sources Files
file(GLOB_RECURSE SOURCE_FILES Private/*.cpp)
file(GLOB_RECURSE HEADER_FILES Private/*.h)

# --- Create the main library ---
add_library(MyGameLibrary)
target_sources(MyGameLibrary PRIVATE ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(MyGameLibrary PUBLIC Public)
target_include_directories(MyGameLibrary PRIVATE Private)
target_link_libraries(MyGameLibrary PRIVATE Vendor Core)

# --- Create the main executable ---
add_executable(MyGame)
target_sources(MyGame PRIVATE ${SOURCE_FILES})
target_include_directories(MyGameLibrary PRIVATE Private)
target_link_libraries(MyGame PRIVATE Vendor Core MyGameLibrary)

# --- Add Compile Definitions ---

# Enable debug mode if we are in the debug build
target_compile_definitions(MyGame PRIVATE $<$<CONFIG:Debug>:DEBUG>)

# --- Tracy Configuration ---
if(TRACY_ENABLE)
    target_compile_definitions(MyGame PRIVATE TRACY_ENABLE)
    target_compile_definitions(MyGameLibrary PRIVATE TRACY_ENABLE)

    if(TRACY_VERBOSE)
        target_compile_definitions(MyGame PRIVATE TRACY_VERBOSE)
        target_compile_definitions(MyGameLibrary PRIVATE TRACY_VERBOSE)
    endif()
endif()
