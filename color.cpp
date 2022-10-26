#include <stdexcept>
#include <vector>
#include "color.h"

Color::Color(int r, int g, int b)
    : rgb{std::vector<int>{r, g, b}}
{
    check_bound();
}

void Color::check_bound(){
    for (int& i: rgb){
        if (i > 255){
            i = 255;
        } else if (i < 0){
            i = 0;
        }
    }
}

void Color::set(int r, int g, int b){
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
    check_bound();
}

int Color::r(){
    return rgb[0];
}

int Color::g(){
    return rgb[1];
}

int Color::b(){
    return rgb[2];
}

int Color::operator[](int i){
    return rgb[i];
}

Color Color::operator+(Color& col){
    Color ret = Color(  rgb[0] + col[0],
                        rgb[1] + col[1],
                        rgb[2] + col[2]);
    ret.check_bound();
    return ret;
}

Color Color::operator-(Color& col){
    Color ret = Color(  rgb[0] + col[0],
                        rgb[1] + col[1],
                        rgb[2] + col[2]);
    ret.check_bound();
    return ret;
}

Color Color::operator*(int mul){
    Color ret = Color(  rgb[0] * mul,
                        rgb[1] * mul,
                        rgb[2] * mul);
    ret.check_bound();
    return ret;
}

Color Color::operator/(int mul){
    Color ret = Color(  rgb[0] / mul,
                        rgb[1] / mul,
                        rgb[2] / mul);
    ret.check_bound();
    return ret;
}
