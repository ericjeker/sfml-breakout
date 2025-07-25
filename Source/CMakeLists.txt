add_subdirectory(Core)
add_subdirectory(Animations)
#add_subdirectory(MiniRts)

# --- Testing Setup ---

# Option to enable/disable building tests.
option(BUILD_TESTS "Build the tests" OFF)

if (BUILD_TESTS)
    # Enable testing for the project
    enable_testing()

    # Add the Tests directory.
    add_subdirectory(TestBed)
endif ()
