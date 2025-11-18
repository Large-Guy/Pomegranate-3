set(PlatformSrc
        Window.cpp
        Window.h
        Common/StandardWindow.cpp
        Common/StandardWindow.h
)

if (APPLE)
    message("Using MacOS backend")

    set(PlatformMacOS
            MacOS/MacOsWindow.cpp
            MacOS/MacOsWindow.h
            MacOS/Effects.mm
            MacOS/Effects.hpp
    )

    set(PlatformSrc ${PlatformSrc} ${PlatformMacOS})
elseif (__LINUX)
    message("Using Linux backend")
    set(PlatformLinux
            Linux/LinuxWindow.cpp
            Linux/LinuxWindow.h
    )

    set(PlatformSrc ${PlatformSrc} ${PlatformMacOS})
elseif (WIN32)
    message("Using Win32 backend")
    set(PlatformWindows
            Windows/WindowsWindow.cpp
            Windows/WindowsWindow.h
    )

    set(PlatformSrc ${PlatformSrc} ${PlatformMacOS})
endif ()

add_library(Platform STATIC
        ${PlatformSrc}
)

if (APPLE)
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
    find_library(QUARTZCORE_FRAMEWORK QuartzCore REQUIRED)
endif ()

target_link_libraries(Platform PUBLIC Core Dependencies ${COCOA_FRAMEWORK} ${QUARTZCORE_FRAMEWORK})