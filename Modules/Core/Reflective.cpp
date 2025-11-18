#include "Reflective.h"

bool Reflective::has(const std::string& str) {
    return data.contains(str);
}

void* Reflective::getPtr(const std::string& str) {
    if (!data.contains(str)) {
        throw std::runtime_error("Reflective object does not have key!");
    }
    return this + data[str].offset;
}

void Reflective::setPtr(const std::string& str, const void* newValue) {
    if (!data.contains(str)) {
        throw std::runtime_error("Reflective object does not have key!");
    }
    std::memcpy(reinterpret_cast<char*>(this) + data[str].offset, newValue, data[str].size);
}

void Reflective::regPtr(const std::string& str, void* t, size_t size) {
    data[str] = {reinterpret_cast<char*>(this) - static_cast<char*>(t), size};
}

