#ifndef POMEGRANATE_SIGNAL_H
#define POMEGRANATE_SIGNAL_H
#include <functional>

template<typename... Args>
class Signal {
public:
    Signal() = default;

    void operator +=(std::function<void(Args...)> function) {
        functions.push_back(function);
    }

    void operator()(Args... args) {
        for (auto function: functions) {
            function(args...);
        }
    }

private:
    std::vector<std::function<void(Args...)> > functions;
};


#endif //POMEGRANATE_SIGNAL_H
