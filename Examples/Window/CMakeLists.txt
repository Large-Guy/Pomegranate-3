add_executable(Window
        Main.cpp
)

target_link_libraries(Window PRIVATE Dependencies Core Platform Rendering Events Framework)