#ifndef POMEGRANATE_THEME_H
#define POMEGRANATE_THEME_H
#include <memory>
#include <unordered_map>
#include <variant>
#include <string>

#include "Rendering/Color.h"


class Theme {
public:
    using Value = std::variant<int, float, Color>;

    Theme();

    static std::shared_ptr<Theme> dark();

    template<typename T>
    void set(std::string key, T t) {
        values.emplace(key, Value(t));
    }

    template<typename T>
    T get(std::string key) {
        if (values.contains(key)) {
            return std::get<T>(values[key]);
        } else {
            return T{};
        }
    }

private:
    std::unordered_map<std::string, Value> values = {};
};


#endif //POMEGRANATE_THEME_H
