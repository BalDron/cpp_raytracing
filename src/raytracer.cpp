#include <iostream>
#include <memory>
#include <cmath>
#include <string>
#include <vector>
    using std::vector;
#include <stdexcept>
    using std::runtime_error;
#include <cstdlib>
#include <thread>
    using std::thread;
#include <functional>
    using std::ref;

#include <iostream>
    using std::cout;

#include "vector3.h"
#include "color.h"
#include "ray.h"
#include "component.h"
#include "object.h"

#include "raytracer.h"

Raytracer::Raytracer(int res_x, int res_y, int depth, int thr){
    resolution_x = res_x;
    resolution_y = res_y;
    ray_depth = depth;
    threads = thr;
    colors = vector<vector<Color>>(res_y);
    for (int i = 0; i < res_y; ++i){
        colors[i].resize(res_x);
    }
}

Vector3 Raytracer::reflect(Vector3& ray_dir, Vector3& normal){
    ray_dir.unite();
    return ray_dir - normal * (2.0 * dot(ray_dir, normal));
}

Color Raytracer::cast_ray(Ray& ray, World& objs, int depth){
    HitRecord record;
    record.color = objs.get_bg();
    record = objs.check_intersections(ray, record);
    if (record.t > 0.0 && depth <= ray_depth){
        Color self_col;
        Color refl_col;
        double self_mul = 0.0;
        double mirror = 0.0;
        double light = 0.0;

        record = objs.intersect(ray, record);
        self_col = record.color;
        self_mul = objs[record.ind].get_material().get_self();
        mirror = objs[record.ind].get_material().get_mirror();

        Vector3 refl_v = reflect(ray.get_direction(), record.hit_normal);
        refl_v.unite();
        Ray new_ray{record.hit_point, refl_v};

        refl_col = cast_ray(new_ray, objs, depth + 1);
        light += mirror * (refl_col.r() + refl_col.g() + refl_col.b()) / (3 * 255.0);

        for (int i = 0; i < objs.lights_number(); ++i){
            if (objs.get_light(i).get_light_force() == 0.0){
                continue;
            }
            Vector3 vec_to_light = objs.get_light(i).get_transform().pos() - record.hit_point;
            Vector3 vec_to_light_copy = vec_to_light;
            vec_to_light.unite();
            double d = dot(vec_to_light, record.hit_normal);
            if (d > 0.0){
                Ray ray_to_light;
                ray_to_light.set_origin(record.hit_point);
                ray_to_light.set_direction(vec_to_light);
                HitRecord new_record;
                new_record = objs.check_intersections(ray_to_light, new_record);
                if (new_record.t < 0.0){
                    light += objs.get_light(i).get_light_force() * d / 255.0;
                }
            }
        }
        record.color = self_col * self_mul * light + refl_col * mirror;
    }
    return record.color;
}


void Raytracer::make_concurrent_step(World& objs, int start, int step){
    int cam = objs.find_camera();
    if (cam == -1){
        throw runtime_error(
            "Raytracer.make_step(): There is no camera");
    }
    Color clr;
    for (int i = start; i < resolution_y; i += step){
        for (int j = 0; j < resolution_x; ++j){
            colors[i][j].set(0, 0, 0);
            int ns = 1;
            int max = 1;
            for (int k = 0; k < ns; ++k){
                double rand_num1, rand_num2;
                rand_num1 = double(std::rand()%max)/ double(max);
                rand_num2 = double(std::rand()%max)/ double(max);
                double ny = (double (i) + rand_num1) / double(resolution_y);
                double nx = (double (j) + rand_num2) / double(resolution_x);
                Ray ray = objs[cam].get_camera().get_ray(
                    nx, ny,
                    objs[cam].get_transform()
                );
                clr = cast_ray(ray, objs, 0);
                colors[i][j] = colors[i][j] + clr;
            }
            colors[i][j] = colors[i][j] / double(ns);
        }
    }
}


void Raytracer::make_step(World& objs, int level){
    thread t { &Raytracer::make_concurrent_step, this, ref(objs), level, threads };
    if (level < threads){
        make_step(objs, level + 1);
    }
    t.join();
}

vector<vector<Color>>& Raytracer::get_image(){
    return colors;
}

void Raytracer::soften_image(){
    double neighbor = 2.0/18.0;
    double far_neighbor = 2.0/36.0;
    double center = 1.0 - 4 * neighbor - 4 * far_neighbor;
    vector<vector<Color>> new_c;
    new_c.resize(resolution_y);
    for (int i = 0; i < resolution_y; ++i){
        new_c[i].resize(resolution_x);
    }
    for (int i = 1; i < resolution_y - 1; ++i){
        for (int j = 1; j < resolution_x - 1; ++j){
            new_c[i][j] = colors[i][j] * center;
            new_c[i][j] = new_c[i][j] +
                (colors[i-1][j] + colors[i+1][j] + colors[i][j-1] + colors[i][j+1]) * neighbor +
                (colors[i-1][j-1] + colors[i+1][j+1] + colors[i+1][j-1] + colors[i-1][j+1]) * far_neighbor;
        }
    }
    for (int i = 1; i < resolution_y - 1; ++i){
        for (int j = 1; j < resolution_x - 1; ++j){
            colors[i][j] = new_c[i][j];
        }
        new_c[i].clear();
    }
    new_c.clear();

}
