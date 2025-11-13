add_library(Layout STATIC
        Container.cpp
        Container.h
        Vertical.cpp
        Vertical.h
        Flexable.cpp
        Flexable.h
        Horizontal.cpp
        Horizontal.h
        Fill.cpp
        Fill.h
)

target_link_libraries(Layout PUBLIC)