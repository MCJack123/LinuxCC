class BitAPI {
public:
    int blshift(int n, int bits) {return n << bits;}
    int brshift(int n, int bits);
    int blogic_rshift(int n, int bits) {return n >> bits;}
    int bxor(int m, int n) {return m ^ n;}
    int bor(int m, int n) {return m | n;}
    int band(int m, int n) {return m & n;}
    int bnot(int n) {return !n;}  
};

extern BitAPI bit;