#include "parallel.hpp"

ParallelAPI parallel;

template<coro_func ... Args>
std::vector<coroutine_t> getCoros(coro_func first, coro_func Args...) {
    std::vector<coroutine_t> coros = getCoros(Args...);
    coros.push_back(coroutine.create(first));
    return coros;
}

std::vector<coroutine_t> getCoros(coro_func first) {
    std::vector<coroutine_t> coros;
    coros.push_back(coroutine.create(first));
    return coros;
}

template<coro_func ... Args>
int ParallelAPI::waitForAny(coro_func first, coro_func Args...) {
    std::vector<coroutine_t> coros = getCoros(first, Args...);
    while (true) {
        for (int i = 0; i < coros.size(); i++) {
            coroutine.resume(coros[i]);
            if (coroutine.status(coros[i]) == "dead") return i;
        }
    }
}

template<coro_func ... Args>
void ParallelAPI::waitForAll(coro_func first, coro_func Args...) {
    std::vector<coroutine_t> coros = getCoros(first, Args...);
    while (coros.size() > 0) {
        for (int i = 0; i < coros.size(); i++) {
            coroutine.resume(coros[i]);
            if (coroutine.status(coros[i]) == "dead") coros.erase(i--);
        }
    }
}