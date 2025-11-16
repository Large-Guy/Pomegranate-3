set(UISrc
        UIElement.cpp
        UIElement.h
        UILayer.cpp
        UILayer.h
        UICompositor.cpp
        UICompositor.h
        Elements/Panel.cpp
        Elements/Panel.h
        Elements/VerticalLayout.cpp
        Elements/VerticalLayout.h
)

add_library(UI STATIC
        ${UISrc}
)

target_link_libraries(UI PUBLIC Core Dependencies Platform Rendering)
