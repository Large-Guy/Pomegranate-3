add_library(Dependencies INTERFACE)

add_subdirectory(SDL3)

target_link_libraries(Dependencies INTERFACE
    SDL3::SDL3
)