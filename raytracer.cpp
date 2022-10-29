#include <iostream>
#include <memory>
#include <string>
#include <vector>
    using std::vector;
#include <stdexcept>
    using std::runtime_error;

#include <iostream>
    using std::cout;

#include "vector3.h"
#include "color.h"
#include "ray.h"
#include "component.h"
#include "object.h"

#include "raytracer.h"

Raytracer::Raytracer(int res_x, int res_y){
    resolution_x = res_x;
    resolution_y = res_y;
    colors = vector<vector<Color>>(res_y);
    for (int i = 0; i < res_y; ++i){
        colors[i].resize(res_x);
    }
}

Color Raytracer::cast_ray(Ray& ray, World& objs, int depth){
    HitRecord record;
    bool there_are_intersections;
    there_are_intersections = objs.check_intersections(ray);
    // std::cout << ray.get_origin()[0] << " " << ray.get_origin()[1] << " " << ray.get_origin()[2] << "\n";
    if (there_are_intersections){
        record = objs.intersect(ray);
    } else {
        record.t = -1.0;
        record.color = objs.get_bg();
    }
    return record.color;
}

int Raytracer::make_step(World& objs){
    int cam_ind = objs.find_camera();
    if (cam_ind == -1){
        throw runtime_error(
            "Raytracer.make_step(): There is no camera");
    }
    int cam = objs.find_camera();
    for (int i = 0; i < resolution_y; ++i){
        for (int j = 0; j < resolution_x; ++j){
            Color color = Color();
            double ny = (double) i / resolution_y;
            double nx = (double) j / resolution_x;
            Ray ray = objs[cam].get_camera().get_ray(
                nx, ny,
                objs[cam].get_transform()
            );
            // std::cout << ray.get_direction()[0] << " " << ray.get_direction()[1] << " " << ray.get_direction()[2] << "\n";
            colors[i][j] = cast_ray(ray, objs, 0);
        }
    }
    return 0;
}

vector<vector<Color>>& Raytracer::get_image(){
    return colors;
}
