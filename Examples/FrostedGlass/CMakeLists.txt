add_executable(FrostedGlass
        Main.cpp
)

target_link_libraries(FrostedGlass PRIVATE Dependencies Core Platform Rendering Events Framework Layout)