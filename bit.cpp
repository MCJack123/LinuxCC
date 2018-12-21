#include "bit.hpp"

BitAPI bit;

int BitAPI::brshift(int n, int bits) {
    int ibit = n & 0x8000;
    while (bits >= 0) n = (n >> (bits - --bits)) | ibit;
    return n;
}