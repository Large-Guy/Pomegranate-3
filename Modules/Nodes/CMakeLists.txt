set(NodesSrc
        Node.cpp
        Node.h
)

add_library(Nodes STATIC
        ${NodesSrc}
)

target_link_libraries(Nodes PUBLIC Core)
