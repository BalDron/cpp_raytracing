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
    return -ray_dir + normal / 2 * std::abs(dot(ray_dir, normal));
}

Color Raytracer::cast_ray(Ray& ray, World& objs, int depth, HitRecord& prev_rec){
    HitRecord record;
    record = objs.check_intersections(ray, record);
    record.color.set(0, 0, 0);
    if (record.t > 0.0 && depth <= ray_depth){
        record = objs.intersect(ray, record);
        Ray new_ray{record.hit_point, reflect(ray.get_direction(), record.hit_normal)};
        Color addit_col = cast_ray(new_ray, objs, depth + 1, record);
        record.color.lighten(addit_col * objs[record.ind].get_material().get_self());
        addit_col = addit_col * objs[record.ind].get_material().get_mirror();
        record.color = record.color + addit_col;
        // if (prev_rec.ind == 1 && record.ind == 2 && record.color.r() == 0 && record.color.b() == 0 && record.color.g() == 0){
        //     std::cout << "bingo\n";
        // }

    } else if (depth == 0){
            record.color = objs.get_bg();
    } else if (depth <= ray_depth){
        for (int i = 0; i < objs.lights_number(); ++i){
            Vector3 ray_to_light = objs.get_light(i).get_transform().pos() - ray.get_origin();
            ray_to_light.unite();
            double d = dot(ray_to_light, prev_rec.hit_normal);
            if (d > 0.0){
                Ray to_light{ray.get_origin(), ray_to_light};
                record = objs.check_intersections(to_light, record);
                Color tmp_col;
                if (record.t <= 0.0){
                    tmp_col = objs.get_bg();
                    tmp_col.lighten(objs.get_light(i).get_light_force() * d / 255.0);
                    record.color = record.color + tmp_col;
                }
            }
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
