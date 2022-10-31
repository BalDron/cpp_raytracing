#include <iostream>
#include <stdexcept>
#include <vector>
    using std::vector;
#include <math.h>
#include "vector3.h"

Vector3::Vector3(double a, double b, double c):
    axis{std::vector<double> {a, b, c}}
{}

Vector3::Vector3(const Vector3& vec):
    axis{std::vector<double> {vec.x(), vec.y(), vec.z()}}
{}

Vector3& Vector3::operator=(const Vector3& vec){
    axis.clear();
    axis.push_back(vec[0]);
    axis.push_back(vec[1]);
    axis.push_back(vec[2]);
    return *this;
}

Vector3::Vector3(const Vector3&& vec){
    axis[0] = vec[0];
    axis[1] = vec[1];
    axis[2] = vec[2];
    // vec.clean();
}
Vector3& Vector3::operator=(Vector3&& vec){
    axis.clear();
    axis.push_back(vec[0]);
    axis.push_back(vec[1]);
    axis.push_back(vec[2]);
    return *this;
}

Vector3::Vector3():
    axis{std::vector<double> {0.0, 0.0, 0.0}}
{}

Vector3::~Vector3(){
    axis.clear();
}

void Vector3::set_x(double a){
    axis[0] = a;
}

void Vector3::set_y(double b){
    axis[1] = b;
}

void Vector3::set_z(double c){
    axis[2] = c;
}

double Vector3::x() const{
    double ret_x = axis[0];
    return ret_x;
}

double Vector3::y() const{
    double ret_y = axis[1];
    return ret_y;
}

double Vector3::z() const{
    double ret_z = axis[2];
    return ret_z;
}

double Vector3::operator[](int i) const{
    double ret_i = axis[i];
    return ret_i;
}

Vector3 Vector3::operator+(const Vector3& vec){
    return Vector3( axis[0] + vec[0],
                    axis[1] + vec[1],
                    axis[2] + vec[2]);
}

Vector3 Vector3::operator-(const Vector3& vec){
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

bool Vector3::operator!=(const Vector3 vec){
    if (x() == vec.x() && y() == vec.y() && z() == vec.z()){
        return false;
    }
    return true;
}

double Vector3::len(){
    return sqrt(axis[0]*axis[0] +
                axis[1]*axis[1] +
                axis[2]*axis[2]);
}

void Vector3::unite(){
    set_x(x()/len());
    set_y(y()/len());
    set_z(z()/len());
}

void Vector3::print(){
    std::cout << x() << " " << y() << " " << z() << "\n";
}

double dot(Vector3& vec1, Vector3& vec2){
    return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
}

Vector3 unit_vector(Vector3& vec){
    return vec / vec.len();
}

void Vector3::clean(){
    axis.clear();
}
