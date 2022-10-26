#include "raytracer.h"

Ray::Ray(const Vector3& orig, const Vector3& dir)
    : origin{orig}, direction{dir} {}

Vector3 Ray::at(double t){
    return origin + direction * t;
}
