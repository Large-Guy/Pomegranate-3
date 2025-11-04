//
// Created by Ravi Lebgue on 11/3/25.
//

#ifndef POMEGRANATE_BITFLAG_H
#define POMEGRANATE_BITFLAG_H
#include <cstdint>

template <typename Enum>
class BitFlag {
    uint32_t flags = {};

    class BitProxy {
        BitFlag& flags;
        Enum flag;
    public:
        BitProxy(BitFlag& flags, Enum flag) : flags(flags), flag(flag) {}

        BitProxy& operator =(bool value) {
            flags.set(flag, value);
            return *this;
        }
        
        operator bool() const {
            return flags.has(flag);
        }
    };
    
public:
    void set(Enum flag, bool value) {
        if (value)
            flags |= 1 << static_cast<Enum>(flag);
        else
            flags &= ~(1 << static_cast<Enum>(flag)); 
    }

    bool has(Enum flag) const {
        return (flags & (1 << static_cast<Enum>(flag))) != 0; 
    }

    void toggle(Enum flag) {
        set(flag, !get(flag));
    }

    bool operator [](Enum flag) const {
        return has(flag);
    }

    BitProxy operator[](Enum flag) {
        return BitProxy(*this, flag);
    }
};


#endif //POMEGRANATE_BITFLAG_H