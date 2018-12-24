#include "math.hpp"

MathAPI math;

std::pair<double, int> MathAPI::frexp(double x) {
    std::pair<double, int> retval;
    retval.first = ::frexp(x, &retval.second);
    return retval;
}

template<typename T, typename ... Args>
T MathAPI::max(T first, Args... args) {
    T m = max(args...);
    if (first > m) return first;
    else return m;
}

template<typename T, typename ... Args>
T MathAPI::min(T first, Args... args) {
    T m = min(args...);
    if (first < m) return first;
    else return m;
}

std::pair<int, double> MathAPI::modf(double x) {
    std::pair<int, double> retval;
    retval.first = floor(x);
    retval.second = x - retval.first;
    return retval;
}