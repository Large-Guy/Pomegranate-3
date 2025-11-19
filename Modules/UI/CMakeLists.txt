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
        Elements/HorizontalLayout.cpp
        Elements/HorizontalLayout.h
        Theme.cpp
        Theme.h
        Elements/Root.cpp
        Elements/Root.h
        Elements/BlurPanel.cpp
        Elements/BlurPanel.h
        Elements/Spacer.cpp
        Elements/Spacer.h
        Elements/Full.cpp
        Elements/Full.h
        Elements/TextInput.cpp
        Elements/TextInput.h
        Elements/Text.cpp
        Elements/Text.h
)

add_library(UI STATIC
        ${UISrc}
)

target_link_libraries(UI PUBLIC Core Dependencies Platform Rendering Nodes)
