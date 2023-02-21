#include <vector>
    using std::vector;
#include "vector3.h"
#include "ray.h"

Ray::Ray(){
    origin.set(0, 0, 0);
    direction.set(0, 0, 0);
}

Ray::Ray(const Vector3& orig, const Vector3& dir):
    origin{orig},
    direction{dir}
{}

Vector3 Ray::at(double t){
    return origin + direction * t;
}

void Ray::set_origin(const Vector3 new_orig){
    origin = new_orig;
}

void Ray::set_direction(const Vector3 new_dir){
    direction = new_dir;
}

Vector3& Ray::get_origin(){
    return origin;
}

Vector3& Ray::get_direction(){
    return direction;
}
