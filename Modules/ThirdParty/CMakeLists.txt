add_library(Dependencies INTERFACE)

add_subdirectory(SDL3)
add_subdirectory(SDL_ttf)
add_subdirectory(SDL_image)

set(SDLSHADERCROSS_VENDORED ON)

add_subdirectory(SDL_shadercross)

target_link_libraries(Dependencies INTERFACE
        SDL3::SDL3
        SDL3::Headers
        SDL3_shadercross::SDL3_shadercross
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
)
