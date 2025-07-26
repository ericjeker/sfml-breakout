cmake_minimum_required(VERSION 3.31)
project(sfml_sandbox_008_animations)

# --- CMake Configuration ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Platform-specific configurations ---
if (UNIX AND NOT APPLE)
    # Linux-specific settings
    find_package(PkgConfig REQUIRED)
    find_package(X11 REQUIRED)

    # Additional Linux libraries that might be needed
    find_package(Threads REQUIRED)
elseif (WIN32)
    # Windows-specific settings
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif ()

# --- Add the Source and Vendor subdirectories ---
add_subdirectory(Vendor)
add_subdirectory(Source)

# --- Copy the Assets folder ---
file(
        COPY
        ${CMAKE_SOURCE_DIR}/Assets
        DESTINATION
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# --- Installation Rules ---
install(TARGETS MyGame
    RUNTIME DESTINATION bin  # For .exe/.dll
    LIBRARY DESTINATION lib  # For .lib
    ARCHIVE DESTINATION lib  # For .a
)
