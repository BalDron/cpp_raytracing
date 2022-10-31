#include <iostream>
#include <memory>
#include <cmath>
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

Raytracer::Raytracer(int res_x, int res_y, int depth){
    resolution_x = res_x;
    resolution_y = res_y;
    ray_depth = depth;
    colors = vector<vector<Color>>(res_y);
    for (int i = 0; i < res_y; ++i){
        colors[i].resize(res_x);
    }
}

Vector3 Raytracer::reflect(Vector3& ray_dir, Vector3& normal){
    return ray_dir - normal / 2 * std::abs(dot(ray_dir, normal));
}

Color Raytracer::cast_ray(Ray& ray, World& objs, int depth, HitRecord prev_rec = HitRecord()){
    // if (prev_rec.hit_normal.len() > 0.0){
    //     prev_rec.hit_normal.print();
    // }
    HitRecord record;
    IntersectionInfo info;
    info = objs.check_intersections(ray);
    if (info.t > 0.0){
        record = objs.intersect(ray, info.t, info.ind);
        if (depth < ray_depth && record.hit_point != prev_rec.hit_point){
            Ray new_ray{record.hit_point, reflect(ray.get_direction(), record.hit_normal)};
            Color addit_col = cast_ray(new_ray, objs, depth + 1, record) / 2;
            record.color = record.color / 2 + addit_col;
        }
    } else {
        if (depth > 0){
            record.color.set(0, 0, 0);
            for (int i = 0; i < objs.lights_number(); ++i){
                Vector3 ray_to_light = objs.get_light(i).get_transform().pos() - ray.get_origin();
                double d = dot(ray_to_light, prev_rec.hit_normal);
                if (d > 0.0){
                    Ray to_light{ray.get_origin(), ray_to_light};
                    Color tmp_col;
                    info = objs.check_intersections(to_light);
                    if (info.t < 0.0){
                        tmp_col = prev_rec.color * objs.get_light(i).get_light_force();
                        tmp_col = tmp_col * d;
                        record.color = record.color + tmp_col;
                    } else {
                        HitRecord tmp_rec = objs.intersect(to_light, info.t, info.ind);
                        tmp_rec.color = tmp_rec.color * 0.25;
                        tmp_rec.color = prev_rec.color * 0.25 + tmp_rec.color;
                        record.color = record.color + tmp_rec.color;
                    }
                } else {
                    record.color.set(0, 0, 0);
                }
            }
        } else {
            record.color = objs.get_bg();
        }
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
