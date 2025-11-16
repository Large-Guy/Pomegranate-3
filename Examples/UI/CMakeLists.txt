add_executable(UIExample
        Main.cpp
)

target_link_libraries(UIExample PRIVATE Dependencies Core Platform Rendering Events Framework Layout UI)
