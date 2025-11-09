add_library(Framework STATIC
        App.cpp
        App.h
)

target_link_libraries(Framework PUBLIC Dependencies Events)