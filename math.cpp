#include "math.hpp"

MathAPI math;

struct frexp_val MathAPI::frexp(double x) {
    struct frexp_val retval;
    retval.m = ::frexp(x, &retval.e);
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

struct modf_val MathAPI::modf(double x) {
    struct modf_val retval;
    retval.i = floor(x);
    retval.f = x - retval.i;
    return retval;
}