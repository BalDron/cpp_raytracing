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

Color Raytracer::cast_ray(Ray& ray, World& objs, int depth, HitRecord& prev_rec){
    // if (prev_rec.hit_normal.len() > 0.0){
    //     prev_rec.hit_normal.print();
    // }
    HitRecord record;
    record = objs.check_intersections(ray, record);
    if (record.t > 0.0){
        record = objs.intersect(ray, record);
        if (depth < ray_depth){
            Ray new_ray{record.hit_point, reflect(ray.get_direction(), record.hit_normal)};
            Color addit_col = cast_ray(new_ray, objs, depth + 1, record)/2;
            record.color = addit_col;
            // record.color = record.color;
            // record.color = record.color / 2 + addit_col;
        }
    } else {
        if (depth > 0){
            record.color.set(0, 0, 0);
            for (int i = 0; i < objs.lights_number(); ++i){
                Vector3 ray_to_light = objs.get_light(i).get_transform().pos() - ray.get_origin();
                double d = dot(ray_to_light, prev_rec.hit_normal);
                if (d >= 0.0){
                    Ray to_light{ray.get_origin(), ray_to_light};
                    Color tmp_col;
                    record = objs.check_intersections(to_light, record);
                    if (record.t <= 0.0){
                        tmp_col = prev_rec.color * (objs.get_light(i).get_light_force()* d);
                        record.color = record.color + tmp_col;
                    } else {
                        // HitRecord tmp_rec;
                        // tmp_rec.t = record.t;
                        // tmp_rec.ind = record.ind;
                        // tmp_rec = objs.intersect(to_light, record);
                        // tmp_rec.color = tmp_rec.color * 0.25;
                        // tmp_rec.color = prev_rec.color * 0.25 + tmp_rec.color;
                        // record.color = record.color + tmp_rec.color;

                        tmp_col = prev_rec.color * 0.5;
                        record.color = record.color + tmp_col;
                    }
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
    HitRecord hit;
    hit.t = 0.0;
    hit.ind = 0;
    hit.color = Color();
    hit.hit_point = Vector3();
    hit.hit_normal = Vector3(1.0, 1.0, 1.0);
    Color clr;
    for (int i = 0; i < resolution_y; ++i){
        for (int j = 0; j < resolution_x; ++j){
            double ny = (double) i / resolution_y;
            double nx = (double) j / resolution_x;
            Ray ray = objs[cam].get_camera().get_ray(
                nx, ny,
                objs[cam].get_transform()
            );
            // std::cout << ray.get_direction()[0] << " " << ray.get_direction()[1] << " " << ray.get_direction()[2] << "\n";
            clr = cast_ray(ray, objs, 0, hit);
            colors[i][j].set(0, 0, 0);
            colors[i][j] = colors[i][j] + clr;
        }
    }
    return 0;
}

vector<vector<Color>>& Raytracer::get_image(){
    return colors;
}
