add_library(Dependencies INTERFACE)

add_subdirectory(SDL3)

set(SDLSHADERCROSS_VENDORED ON)

add_subdirectory(SDL_shadercross)

target_link_libraries(Dependencies INTERFACE
        SDL3::SDL3
        SDL3::Headers
        SDL3_shadercross::SDL3_shadercross
)