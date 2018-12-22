#include "coroutine.hpp"

class ParallelAPI {
public:
    template<coro_func ... Args>
    int waitForAny(coro_func first, coro_func Args...);
    template<coro_func ... Args>
    void waitForAll(coro_func first, coro_func Args...);
};

extern ParallelAPI parallel;