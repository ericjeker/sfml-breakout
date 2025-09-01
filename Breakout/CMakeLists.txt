# --- Options and Configurations ---
option(TRACY_ENABLE "Enable Tracy profiler" ON)
option(TRACY_VERBOSE "Enable Tracy verbose output" ON)

# --- Sources Files ---
file(GLOB_RECURSE SOURCE_FILES Source/*.cpp)
file(GLOB_RECURSE HEADER_FILES Source/*.h)

set(EXECUTABLE_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Source/Main.cpp")
list(REMOVE_ITEM SOURCE_FILES ${EXECUTABLE_SOURCE})

# --- Create the main library, we build a library so import it in the TestBed ---
add_library(BreakoutLibrary)
target_sources(BreakoutLibrary PRIVATE ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(BreakoutLibrary PUBLIC Source)
target_link_libraries(BreakoutLibrary PRIVATE Vendor Core)

# --- Create the main executable ---
add_executable(Breakout)
target_sources(Breakout PRIVATE ${EXECUTABLE_SOURCE})
target_link_libraries(Breakout PRIVATE Vendor Core BreakoutLibrary)

# Enable debug mode if we are in the debug build
target_compile_definitions(BreakoutLibrary PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_compile_definitions(Breakout PRIVATE $<$<CONFIG:Debug>:DEBUG>)

# --- Tracy Configuration ---
if (TRACY_ENABLE)
    target_compile_definitions(Breakout PRIVATE TRACY_ENABLE)
    target_compile_definitions(BreakoutLibrary PRIVATE TRACY_ENABLE)
    if (TRACY_VERBOSE)
        target_compile_definitions(Breakout PRIVATE TRACY_VERBOSE)
        target_compile_definitions(BreakoutLibrary PRIVATE TRACY_VERBOSE)
    endif ()
endif ()

# --- Post-build assets copy for build-tree runs ---
add_custom_command(TARGET Breakout POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/Assets
          $<TARGET_FILE_DIR:Breakout>/Assets
  COMMENT "Copying assets to build directory"
)

add_custom_command(TARGET Breakout POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/Content
          $<TARGET_FILE_DIR:Breakout>/Content
  COMMENT "Copying content to build directory"
)

# --- Testing toggle integrates with top-level CTest ---
option(BUILD_TESTS "Build the tests" OFF)
if (BUILD_TESTS AND BUILD_TESTING)
    enable_testing()
    add_subdirectory(TestBed)
endif ()
