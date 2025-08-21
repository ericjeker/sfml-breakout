# --- Sources Files
file(GLOB_RECURSE SOURCE_FILES Private/*.cpp)
file(GLOB_RECURSE HEADER_FILES Private/*.h)

add_library(Core)
target_sources(Core PRIVATE ${SOURCE_FILES} ${HEADER_FIELS})
target_include_directories(Core PUBLIC Public)
target_include_directories(Core PRIVATE Private)
target_link_libraries(Core PRIVATE Vendor)

# Enable debug mode if we are in the debug build
target_compile_definitions(Core PRIVATE $<$<CONFIG:Debug>:DEBUG>)
