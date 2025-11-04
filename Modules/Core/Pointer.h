#ifndef POMEGRANATE_3_POINTER_H
#define POMEGRANATE_3_POINTER_H
#include <memory>

/// Thin wrapper to make things less verbose
template <typename T>
class Pointer {
    std::shared_ptr<T> ptr;
public:
    Pointer(std::nullptr_t null) : ptr(null) {
    }

    Pointer(T* ptr) : ptr(ptr) {
    }

    Pointer(std::shared_ptr<T> ptr) : ptr(ptr) {
    }

    Pointer(const Pointer& other) : ptr(other.ptr) {
    }

    template <typename Cast>
    operator Pointer<Cast>() {
        return Pointer<Cast>(ptr);
    }

    T* get() {
        return ptr.get();
    }

    T* operator->() {
        return ptr.get();
    }
};

template <typename Type, typename... Args>
static Pointer<Type> newPtr(Args... args)
{
    Pointer<Type> made = std::make_shared<Type>(std::forward<Args>(args)...);
    return made;
}

#endif //POMEGRANATE_3_POINTER_H