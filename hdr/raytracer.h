#ifndef __RAYTRACER__
#define __RAYTRACER__

#include <vector>
using std::vector;

class World;
class Ray;
class Vector3;
class Color;

class Raytracer{
private:
    int resolution_x;
    int resolution_y;
    int ray_depth;
    int threads;
    vector<vector<Color>> colors;
public:
    Raytracer(int res_x, int rex_y, int depth, int thr);
    void make_step(World& objs, int level);
    void make_concurrent_step(World& objs, int start, int step);
    Color cast_ray(Ray& ray, World& objs, int depth);
    Vector3 reflect(Vector3& ray_dir, Vector3& normal);
    vector<vector<Color>>& get_image();
    void soften_image();
};

#endif
