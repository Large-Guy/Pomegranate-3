#include "Theme.h"

Theme::Theme() = default;

std::shared_ptr<Theme> Theme::dark() {
    auto theme = std::make_shared<Theme>();
    theme->set("background", Color{0.0f});

    theme->set("primary-fill", Color{0.122f, 0.118f, 0.113f});
    theme->set("primary-border", Color{0.236f, 0.231f, 0.218f});

    theme->set("secondary-fill", Color{0.140f, 0.140f, 0.149f});
    theme->set("secondary-border", Color{0.255f, 0.255f, 0.238f});

    theme->set("neutral-fill", Color{0.310f, 0.310f, 0.302f});
    theme->set("neutral-border", Color{0.380f, 0.380f, 0.365f});

    theme->set("accent-fill", Color{0.666f, 0.325f, 0.180f});
    theme->set("accent-border", Color{0.666f, 0.325f, 0.180f});
    return theme;
}
