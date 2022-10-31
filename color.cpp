#include <stdexcept>
#include <vector>
    using std::vector;
#include "color.h"

Color::Color():
    rgb{vector<int>{0, 0, 0}}
{}

Color::Color(int r, int g, int b):
    rgb{vector<int>{r, g, b}}
{
    // check_bound();
}

Color::Color(double r, double g, double b):
    rgb{vector<int>{int(r), int(g), int(b)}}
{
    // check_bound();
}

Color::Color(const Color& col):
    rgb{vector<int>{col.r(), col.g(), col.b()}}
{}

Color::~Color(){
    rgb.clear();
}

void Color::check_bound(){
    for (int i = 0; i < 3; ++i){
        if (rgb[i] > 255){
            rgb[i] = 255;
        } else if (rgb[i] < 0){
            rgb[i] = 0;
        }
    }
}

void Color::set(int r, int g, int b){
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
    // check_bound();
}

int Color::r() const{
    int ret_r = rgb[0];
    return ret_r;
}

int Color::g() const{
    int ret_g = rgb[1];
    return ret_g;
}

int Color::b() const{
    int ret_b = rgb[2];
    return ret_b;
}

int Color::operator[](int i) const{
    int ret_i = rgb[i];
    return ret_i;
}

Color Color::operator+(Color& col){
    Color ret = Color(  rgb[0] + col[0],
                        rgb[1] + col[1],
                        rgb[2] + col[2]);
    // ret.check_bound();
    return ret;
}

Color Color::operator-(Color& col){
    Color ret = Color(  rgb[0] + col[0],
                        rgb[1] + col[1],
                        rgb[2] + col[2]);
    // ret.check_bound();
    return ret;
}

Color Color::operator*(int mul){
    Color ret = Color(  rgb[0] * mul,
                        rgb[1] * mul,
                        rgb[2] * mul);
    // ret.check_bound();
    return ret;
}

Color Color::operator*(double mul){
    Color ret = Color(  double(rgb[0]) * mul,
                        double(rgb[1]) * mul,
                        double(rgb[2]) * mul);
    // ret.check_bound();
    return ret;
}

Color Color::operator/(int mul){
    Color ret = Color(  rgb[0] / mul,
                        rgb[1] / mul,
                        rgb[2] / mul);
    // ret.check_bound();
    return ret;
}

Color& Color::operator=(const Color& col){
    rgb.clear();
    rgb.push_back(col.r());
    rgb.push_back(col.g());
    rgb.push_back(col.b());
    return *this;
}
