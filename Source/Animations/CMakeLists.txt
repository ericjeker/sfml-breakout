# --- Options and Configurations ---
option(TRACY_ENABLE "Enable Tracy profiler" ON)
option(TRACY_VERBOSE "Enable Tracy verbose output" ON)

# --- Sources Files
file(GLOB_RECURSE SOURCE_FILES Private/*.cpp)
file(GLOB_RECURSE HEADER_FILES Private/*.h)

# --- Create the main library ---
add_library(AnimationsLibrary)
target_sources(AnimationsLibrary PRIVATE ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(AnimationsLibrary PUBLIC Public)
target_include_directories(AnimationsLibrary PRIVATE Private)
target_link_libraries(AnimationsLibrary PRIVATE Vendor Core)

# --- Create the main executable ---
add_executable(Animations)
target_sources(Animations PRIVATE ${SOURCE_FILES})
target_include_directories(AnimationsLibrary PRIVATE Private)
target_link_libraries(Animations PRIVATE Vendor Core AnimationsLibrary)

# --- Add Compile Definitions ---

# Enable debug mode if we are in the debug build
target_compile_definitions(Animations PRIVATE $<$<CONFIG:Debug>:DEBUG>)

# --- Tracy Configuration ---
if(TRACY_ENABLE)
    target_compile_definitions(Animations PRIVATE TRACY_ENABLE)
    target_compile_definitions(AnimationsLibrary PRIVATE TRACY_ENABLE)

    if(TRACY_VERBOSE)
        target_compile_definitions(Animations PRIVATE TRACY_VERBOSE)
        target_compile_definitions(AnimationsLibrary PRIVATE TRACY_VERBOSE)
    endif()
endif()
