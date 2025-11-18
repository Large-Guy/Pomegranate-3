add_executable(MessagingAppUI
        Main.cpp
)

target_link_libraries(MessagingAppUI PRIVATE Dependencies Core Platform Rendering Events Framework Layout UI)
