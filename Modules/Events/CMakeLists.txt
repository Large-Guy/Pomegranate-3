add_library(Events STATIC
        Event.h
        Event.cpp
)

target_link_libraries(Events PUBLIC Dependencies Platform)