cmake_minimum_required(VERSION 3.31)
project(Breakout)

# --- CMake Specifications ---
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
add_subdirectory(Core)

# --- Games or Sub-Projects ---
add_subdirectory(Breakout)

# --- Installation ---
install(TARGETS Breakout
        EXPORT Breakout
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/test
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
