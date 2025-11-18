#ifndef POMEGRANATE_REFLECTIVE_H
#define POMEGRANATE_REFLECTIVE_H
#include <memory>
#include <string>
#include <unordered_map>

class Reflective {
public:
    bool has(const std::string& str);

    void* getPtr(const std::string& str);

    void setPtr(const std::string& str, const void* newValue);

    template<typename T>
    T* get(const std::string& str) {
        return static_cast<T*>(getPtr(str));
    }

    template<typename T>
    void set(const std::string& str, const T& value) {
        setPtr(str, &value);
    }

protected:
    void regPtr(const std::string& str, void* t, size_t size);

    template<typename T>
    void reg(const std::string& str, T& t) {
        regPtr(str, &t, sizeof(T));
    }

private:
    struct Value {
        std::ptrdiff_t offset;
        size_t size;
    };

    std::unordered_map<std::string, Value> data = {};
};


#endif //POMEGRANATE_REFLECTIVE_H
