class ColorsAPI {
public:
    static const int white = 0x1;
    static const int orange = 0x2;
    static const int magenta = 0x4;
    static const int lightBlue = 0x8;
    static const int yellow = 0x10;
    static const int lime = 0x20;
    static const int pink = 0x40;
    static const int gray = 0x80;
    static const int lightGray = 0x100;
    static const int cyan = 0x200;
    static const int purple = 0x400;
    static const int blue = 0x800;
    static const int brown = 0x1000;
    static const int green = 0x2000;
    static const int red = 0x4000;
    static const int black = 0x8000;

    int combine(int color1, int color2) {return color1 | color2;}
    int subtract(int colors, int color) {return colors & (!color);}
    int test(int colors, int color) {return colors & color;}
};

class ColoursAPI : ColorsAPI {
public:
    static const int grey = gray;
    static const int lightGrey = lightGray;
};

extern ColorsAPI colors;
extern ColoursAPI colours;