add_library(Core STATIC
        Pointer.cpp
        Pointer.h
        App.cpp
        App.h
        BitFlag.cpp
        BitFlag.h
)

target_link_libraries(Core PUBLIC Dependencies)