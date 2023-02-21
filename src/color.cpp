#include <stdexcept>
#include <vector>
    using std::vector;
#include "color.h"

Color::Color():
    rgb{vector<double>{0, 0, 0}}
{}

    Color::Color(double c):
        rgb{vector<double>{c, c, c}}
    {}

Color::Color(double r, double g, double b):
    rgb{vector<double>{r, g, b}}
{}

Color::Color(const Color& col):
    rgb{vector<double>{col.r(), col.g(), col.b()}}
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

void Color::set(double r, double g, double b){
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
}

double Color::r() const{
    double ret_r = rgb[0];
    return ret_r;
}

double Color::g() const{
    double ret_g = rgb[1];
    return ret_g;
}

double Color::b() const{
    double ret_b = rgb[2];
    return ret_b;
}

double Color::operator[](int i) const{
    double ret_i = rgb[i];
    return ret_i;
}

void Color::lighten(double lr, double lg, double lb){
    rgb[0] = rgb[0] * lr;
    rgb[1] = rgb[1] * lg;
    rgb[2] = rgb[2] * lb;
}

void Color::lighten(double l){
    rgb[0] = rgb[0] * l;
    rgb[1] = rgb[1] * l;
    rgb[2] = rgb[2] * l;
}

void Color::lighten(Color l){
    double av = (l[0] + l[1] + l[2])/3;
    rgb[0] = rgb[0] * av / 255.0;
    rgb[1] = rgb[1] * av / 255.0;
    rgb[2] = rgb[2] * av / 255.0;
}

Color Color::operator+(Color col){
    Color ret = Color(  rgb[0] + col[0],
                        rgb[1] + col[1],
                        rgb[2] + col[2]);
    return ret;
}

Color Color::operator-(Color col){
    Color ret = Color(  rgb[0] + col[0],
                        rgb[1] + col[1],
                        rgb[2] + col[2]);
    return ret;
}

Color Color::operator*(double mul){
    Color ret = Color(  rgb[0] * mul,
                        rgb[1] * mul,
                        rgb[2] * mul);
    return ret;
}

Color Color::operator/(double mul){
    Color ret = Color(  rgb[0] / mul,
                        rgb[1] / mul,
                        rgb[2] / mul);
    return ret;
}

Color& Color::operator=(const Color& col){
    rgb.clear();
    rgb.push_back(col.r());
    rgb.push_back(col.g());
    rgb.push_back(col.b());
    return *this;
}
