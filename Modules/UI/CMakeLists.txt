set(UISrc
        UIElement.cpp
        UIElement.h
        UILayer.cpp
        UILayer.h
        UICompositor.cpp
        UICompositor.h
        Elements/PanelElement.cpp
        Elements/PanelElement.h
        Elements/VerticalLayoutElement.cpp
        Elements/VerticalLayoutElement.h
        Elements/HorizontalLayoutElement.cpp
        Elements/HorizontalLayoutElement.h
        Theme.cpp
        Theme.h
        Elements/RootElement.cpp
        Elements/RootElement.h
        Elements/BlurElement.cpp
        Elements/BlurElement.h
        Elements/SpacerElement.cpp
        Elements/SpacerElement.h
        Elements/FullLayoutElement.cpp
        Elements/FullLayoutElement.h
        Elements/InputElement.cpp
        Elements/InputElement.h
        Elements/TextElement.cpp
        Elements/TextElement.h
)

add_library(UI STATIC
        ${UISrc}
)

target_link_libraries(UI PUBLIC Core Dependencies Platform Rendering Nodes)
