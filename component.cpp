#include <iostream>
#include <cmath>
#include <stdexcept>
#include <memory>
#include <string>
#include <vector>
    using std::vector;

#include "color.h"
#include "vector3.h"
#include "ray.h"
#include "component.h"

Component::Component(Component_name n):
    name{n}
{}

Component_name Component::get_name() const{
    return name;
}

Transform::Transform():
    Component{Component_name::transform},
    position{Vector3()},
    rotation{Vector3()}
{}

Transform::Transform(Vector3& pos):
    Component{Component_name::transform},
    position{pos},
    rotation{Vector3()}
{}

void Transform::set_pos(const Vector3& new_pos){
    position = new_pos;
}

Vector3& Transform::pos(){
    return position;
}

Vector3& Transform::rot(){
    return rotation;
}

Camera::Camera():
    Component{Component_name::camera},
    is_active{false}
{}


Camera::Camera( const Vector3 &ll_corn,
                const Vector3 &hor,
                const Vector3 &vert
                ):
    Component{Component_name::camera},
    is_active{true},
    ll_corner{ll_corn},
    horisontal{hor},
    vertical{vert}
{}

void Camera::setup(
        const Vector3 &ll_corn,
        const Vector3 &hor,
        const Vector3 &vert){
    ll_corner = ll_corn;
    horisontal = hor;
    vertical = vert;
}

Ray Camera::get_ray(double nx, double ny, Transform& transform){
    if (!is_active){
        throw std::runtime_error(
            "Camera.get_ray: inactive camera"
        );
    }

    Vector3 direction = (   ll_corner - transform.pos() +
                            vertical * ny +
                            horisontal * nx);
    return Ray(transform.pos(), direction);
}

void Camera::set(bool stat){
    is_active = stat;
}

void Camera::set_active(){
    is_active = true;
}

void Camera::set_inactive(){
    is_active = false;
}

bool Camera::check_active(){
    return is_active;
}

Shape::Shape():
    Component{Component_name::shape}
{}

Shape::Shape(Color col, double rad):
    Component{Component_name::shape},
    type{Shape_type::circle},
    sphere_radius{rad},
    color{col}
{}

void Shape::set_color(Color& col){
    color.set(col.r(), col.g(), col.b());
}

void Shape::set_rad(double rad){
    sphere_radius = rad;
}

Color& Shape::get_color(){
    std::cout << color.r() << " " << color.g() << " " << color.b() << "\n";
    return color;
}

vector<double> Shape::sphere_count_abcd(Ray& ray, Vector3& pos){
    Vector3 ray_orig = ray.get_origin();
    Vector3 ray_dir = ray.get_direction();
    double a = dot(ray_dir, ray_dir);
    double b = 2 * dot(ray_dir, ray_orig - pos);
    double c = dot(ray_orig - pos, ray_orig - pos) - sphere_radius * sphere_radius;
    double d = b * b - 4.0 * a * c;
    return {a, b, c, d};
}

bool Shape::check_intersection(Ray& ray, Transform& transform){
    vector<double> abcd = sphere_count_abcd(ray, transform.pos());
    if (abcd[3] >= 0.0) return true;
    return false;
}

HitRecord Shape::intersect_w_ray(Ray& ray, Transform& transform){
    vector<double> abcd  = sphere_count_abcd(ray, transform.pos());
    double t;
    Vector3 hit_point;
    Vector3 hit_normal;
    HitRecord record;
    if (abcd[3] == 0.0){
        t = -abcd[1] / (2 * abcd[0]);
    } else {
        double t1;
        double t2;
        t1 = (-abcd[1] + std::pow(abcd[3], 0.5)) / (2.0 * abcd[0]);
        t2 = (-abcd[1] - std::pow(abcd[3], 0.5)) / (2.0 * abcd[0]);
        if (t2 >= 0.0){
            t = t2;
        } else {
            t = t1;
        }
    }
    hit_point = ray.at(t);
    hit_normal = unit_vector(hit_point - transform.pos());
    record.t = t;
    record.hit_point = hit_point;
    record.hit_normal = hit_normal;
    record.color = color;
    return record;
}