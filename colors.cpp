#include "colors.hpp"

ColorsAPI colors;
ColoursAPI colours;

std::map<int, int> ColorsAPI::ansiMap = [] {
    std::map<int, int> retval = {
        {white, 15},
        {orange, 14},
        {magenta, 13},
        {lightBlue, 12},
        {yellow, 11},
        {lime, 10},
        {pink, 9},
        {gray, 8},
        {lightGray, 7},
        {cyan, 6},
        {purple, 5},
        {blue, 4},
        {brown, 3},
        {green, 2},
        {red, 1},
        {black, 0}
    };
    return retval;
}();