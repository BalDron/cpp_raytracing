#include <iostream>
#include <stdexcept>
#include <vector>
    using std::vector;
#include <math.h>
#include "vector3.h"

Vector3::Vector3(double a, double b, double c):
    axis{std::vector<double> {a, b, c}}
{}

Vector3::Vector3():
    axis{std::vector<double> {0.0, 0.0, 0.0}}
{}

void Vector3::set_x(double a){
    axis[0] = a;
}

void Vector3::set_y(double b){
    axis[1] = b;
}

void Vector3::set_z(double c){
    axis[2] = c;
}

double Vector3::x(){
    return axis[0];
}

double Vector3::y(){
    return axis[1];
}

double Vector3::z(){
    return axis[2];
}

double Vector3::operator[](int i){
    return axis[i];
}

Vector3 Vector3::operator+(Vector3 vec){
    return Vector3( axis[0] + vec[0],
                    axis[1] + vec[1],
                    axis[2] + vec[2]);
}

Vector3 Vector3::operator-(Vector3 vec){
    return Vector3( axis[0] - vec[0],
                    axis[1] - vec[1],
                    axis[2] - vec[2]);
}

Vector3 Vector3::operator-(){
    return Vector3(-axis[0], -axis[1], -axis[2]);
}

Vector3 Vector3::operator*(double mul){
    return Vector3( axis[0] * mul,
                    axis[1] * mul,
                    axis[2] * mul);
}

Vector3 Vector3::operator/(double mul){
    if (mul == 0.0){
        throw std::runtime_error("Vector3 division by zero");
    }
    return Vector3( axis[0] / mul, axis[1] / mul, axis[2] / mul);
}

double Vector3::len(){
    return sqrt(axis[0]*axis[0] +
                axis[1]*axis[1] +
                axis[2]*axis[2]);
}

Vector3 Vector3::unite(){
    return Vector3( x()/len(),
                    y()/len(),
                    z()/len());
}

void Vector3::print(){
    std::cout << x() << " " << y() << " " << z() << "\n";
}

double dot(Vector3 vec1, Vector3 vec2){
    return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
}

Vector3 unit_vector(Vector3 vec){
    return vec / vec.len();
}
