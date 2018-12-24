#ifndef MATHAPI_HPP
#define MATHAPI_HPP
#include <math.h>
#include <utility>

class MathAPI {
public:
    double abs(double x) {return ::abs(x);}
    double acos(double x) {return ::acos(x);}
    double asin(double x) {return ::asin(x);}
    double atan(double x) {return ::atan(x);}
    double atan2(double y, double x) {return ::atan2(y, x);}
    int ceil(double x) {return (int)::ceil(x);}
    double cos(double x) {return ::cos(x);}
    double cosh(double x) {return ::cosh(x);}
    double deg(double x) {return x * (180.0/M_PI);}
    double exp(double x) {return ::exp(x);}
    int floor(double x) {return (int)::floor(x);}
    double fmod(double x, double y) {return ::fmod(x, y);}
    std::pair<double, int> frexp(double x); // m, e
    double huge = __DBL_MAX__;
    double ldexp(double m, int e) {return ::ldexp(m, e);}
    double log(double x) {return ::log(x);}
    double log10(double x) {return ::log10(x);}
    template<typename T, typename ... Args>
    T max(T first, Args... args);
    template<typename T>
    T max(T first) {return first;}
    template<typename T, typename ... Args>
    T min(T first, Args... args);
    template<typename T>
    T min(T first) {return first;}
    std::pair<int, double> modf(double x); // i, f
    double pi = M_PI;
    double pow(double x, double y) {return ::pow(x, y);}
    double rad(double x) {return x * (M_PI / 180.0);}
    double random() {return (double)rand() / (double)RAND_MAX;}
    int random(int m) {return rand() % m + 1;}
    int random(int m, int n) {return rand() % (m + 1 - n) + n;}
    void randomseed(int x) {srand(x);}
    double sin(double x) {return ::sin(x);}
    double sinh(double x) {return ::sinh(x);}
    double sqrt(double x) {return ::sqrt(x);}
    double tan(double x) {return ::tan(x);}
    double tanh(double x) {return ::tanh(x);}
};

extern MathAPI math;

#endif