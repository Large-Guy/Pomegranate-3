add_library(Core STATIC
        BitFlag.cpp
        BitFlag.h
        Pomegranate.cpp
        Pomegranate.h
        Reflective.cpp
        Reflective.h
        Signal.cpp
        Signal.h
)

target_link_libraries(Core PUBLIC Dependencies)