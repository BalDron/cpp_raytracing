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

Shape::Shape(Shape_type t):
    Component{Component_name::shape},
    type{t}
{}

Shape::Shape(Color col, double rad):
    Component{Component_name::shape},
    type{Shape_type::sphere},
    sphere_radius{rad},
    color{col}
{}

Shape::Shape(Color col, Vector3 n):
    Component{Component_name::shape},
    type{Shape_type::plane},
    color{col}
{
    params.push_back(n);
}

Shape::Shape(Color col, vector<Vector3> coords):
    Component{Component_name::shape},
    type{Shape_type::triangle},
    color{col}
{
    for (auto i: coords){
        params.push_back(i);
    }
}

Shape::~Shape(){
    params.clear();
}

void Shape::set_type(Shape_type t){
    type = t;
}

void Shape::set_color(Color& col){
    color.set(col.r(), col.g(), col.b());
}

void Shape::set_rad(double rad){
    if (type != Shape_type::sphere){
        throw std::runtime_error(
            "Shape.set_rad(): not a "
        );
    }
    sphere_radius = rad;
}

void Shape::set_params(vector<Vector3>& p){
    params.clear();
    for (auto& i: p){
        params.push_back(i);
    }
}

void Shape::add_param(Vector3& p){
    params.push_back(p);
}

void Shape::clear_params(){
    params.clear();
}

Color& Shape::get_color(){
    std::cout << color.r() << " " << color.g() << " " << color.b() << "\n";
    return color;
}

Vector3 Shape::get_normal(const HitRecord& rec, Transform& transform){
    Vector3 n;
    switch (type) {
        case Shape_type::sphere:
            n = rec.hit_point - transform.pos();
            break;
        case Shape_type::plane:
            n = params[0];
            break;
        case Shape_type::triangle:
            Vector3 e0, e1;
            e0 = params[1] - params[0];
            e1 = params[2] - params[0];
            n = cross(e0, e1);
            break;
    }
    n.unite();
    return n;
}

vector<double> Shape::sphere_count_abcd(Ray& ray, Vector3& pos){
    Vector3& ray_orig = ray.get_origin();
    Vector3& ray_dir = ray.get_direction();
    double a = dot(ray_dir, ray_dir);
    Vector3 tmp = ray_orig - pos;
    double b = 2 * dot(ray_dir, tmp);
    double c = dot(tmp, tmp) - sphere_radius * sphere_radius;
    double d = b * b - 4.0 * a * c;
    return {a, b, c, d};
}

double Shape::check_sphere_intersection(Ray& ray, Transform& transform){
    vector<double> abcd = sphere_count_abcd(ray, transform.pos());
    double t = (-abcd[1] - std::pow(abcd[3], 0.5)) / (2.0 * abcd[0]);
    if (abcd[3] > 0.0 && t > 0.001){
        return t;
    }
    return -1.0;
}

double Shape::check_plane_intersection(Ray& ray, Transform& transform){
    double d = -dot(params[0], transform.pos());
    double t = -(dot(params[0], ray.get_origin()) + d) / (dot(params[0], ray.get_direction()));
    if (t > 0.001){
        return t;
    }
    return -1.0;
}

double Shape::check_triangle_intersection(Ray& ray, Transform& transform){
    Vector3 v0, v1, v2;
    v0 = params[0] + transform.pos();
    v1 = params[1] + transform.pos();
    v2 = params[2] + transform.pos();
    Vector3 e0, e1, e2;
    e0 = v1 - v0;
    e1 = v2 - v1;
    e2 = v0 - v2;
    Vector3 n = cross(e0, e1);
    n.unite();
    double d = -dot(n, v0);
    double t = -(dot(n, ray.get_origin()) + d) / dot(n, ray.get_direction());
    v0 = ray.at(t) - v0;
    v0 = cross(e0, v0);
    v1 = ray.at(t) - v1;
    v1 = cross(e1, v1);
    v2 = ray.at(t) - v2;
    v2 = cross(e2, v2);
    if (    dot(n, v0) > 0.0 &&
            dot(n, v1) > 0.0 &&
            dot(n, v2) > 0.0 && t > 0.001){
        return t;
    }
    return -1.0;
}

double Shape::check_intersection(Ray& ray, Transform& transform){
    switch (type) {
        case Shape_type::sphere:
            return check_sphere_intersection(ray, transform);
        case Shape_type::plane:
            return check_plane_intersection(ray, transform);
        case Shape_type::triangle:
            return check_triangle_intersection(ray, transform);
    }

    return -1.0;
}



HitRecord& Shape::intersect_w_ray(Ray& ray, Transform& transform, HitRecord& record){
    record.hit_point = ray.at(record.t);
    record.hit_normal = get_normal(record, transform);
    record.hit_normal.unite();
    record.color = color;
    return record;
}


Material::Material():
    Component{Component_name::material},
    self{1.0}, mirror{0.0}, glass{0.0}
{}

Material::Material(double s, double m):
    Component{Component_name::material},
    self{s}, mirror{m}, glass{1.0 - s - m}
{}

double Material::get_self(){
    return self;
}

double Material::get_mirror(){
    return mirror;
}

double Material::get_glass(){
    return glass;
}

void Material::set(double s, double m, double g){
    self = s;
    mirror = m;
    glass = g;
}
