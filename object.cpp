#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
    using std::vector;

#include "vector3.h"
#include "ray.h"
#include "color.h"
#include "component.h"
#include "object.h"

Component& Object::get_component(Component_name comp_name = Component_name::shape){
    switch (comp_name) {
        case Component_name::transform:
            return transform;
        case Component_name::camera:
            return camera;
        default:
            return shape;
    }
}

Camera& Object::get_camera(){
    return camera;
}

Shape& Object::get_shape(){
    return shape;
}

Transform& Object::get_transform(){
    return transform;
}

Object::Object(int ind, Vector3& pos){
    index = ind;
    transform = Transform(pos);
    camera = Camera();
    shape = Shape();

}

Object::Object(int ind):
    index{ind}, camera{}, transform{}, shape{}
{}

int Object::get_ind(){
    return index;
}

void Object::set_light(double force){
    if (force > 0.0){
        is_light = true;
        light_force = force;
    } else {
        is_light = false;
        light_force = 0.0;
    }
}

bool Object::check_light(){
    return is_light;
}

double Object::get_light_force(){
    return light_force;
}

Object::~Object(){}

World::World():
    index_count{0}
{}

int World::size() const{
    return objects.size();
}

World::~World(){
    objects.clear();
}

int World::new_obj(){
    objects.push_back(std::make_shared<Object>(index_count));
    index_count += 1;
    return index_count - 1;
}

Object& World::get_obj(int ind){
    if (ind < 0 || ind >= size()){
        throw std::runtime_error(
            "World.get_obj(): ind is out of size"
        );
    }
    for (int i = 0; i < objects.size(); ++i){
        if (objects[i]->get_ind() == ind){
            return *objects[i].get();
        }
    }
    throw std::runtime_error(
        "World.get_obj(): object requested not found"
    );
}

Object& World::operator[](int ind){
    if (ind < 0 || ind >= size()){
        throw std::runtime_error(
            "World.operator[](): ind is out of bound"
        );
    }
    for (int i = 0; i < objects.size(); ++i){
        if (objects[i]->get_ind() == ind){
            return *objects[i].get();
        }
    }
    throw std::runtime_error(
        "World.operator[](): object requested not found"
    );
}

int World::find_camera(){
    for (int i = 0; i < objects.size(); ++i){
        if (objects[i]->get_camera().check_active()){
            active_cam = objects[i]->get_ind();
            return active_cam;
        }
    }
    throw std::runtime_error(
        "World.find_camera: no acive camera found"
    );
}

void World::rem_obj(int ind){
    objects.erase(objects.begin() + ind - 1);
}

IntersectionInfo World::check_intersections(Ray& ray){
    double t, tmp_t;
    int ind;
    t = -1.0;
    for (int i = 0; i < size(); ++i){
        if (!objects[i]->get_camera().check_active() && !objects[i]->check_light()){
            tmp_t = objects[i]->get_shape().check_intersection(ray, objects[i]->get_transform());
            if ((tmp_t > 0.0 && tmp_t < t) || t < 0.0){
                t = tmp_t;
                ind = objects[i]->get_ind();
            }
        }
    }
    return {ind, t};
}

HitRecord World::intersect(Ray& ray, double t, int ind){
    HitRecord record, temp;
    // bool record_empty = true;
    // for (int i = 0; i < size(); ++i){
    //     if (!objects[i]->get_camera().check_active() && !objects[i]->check_light()){
    //         double t = objects[i]->get_shape().check_intersection(
    //                 ray, objects[i]->get_transform());
    //         if (t > 0.0){
    //             // std::cout << "int " << i << std::endl;
    //             temp = objects[i]->get_shape().intersect_w_ray(
    //                     ray, objects[i]->get_transform(), t);
    //             if (record_empty || temp.t <= record.t){
    //                 record = temp;
    //                 record_empty = false;
    //             }
    //         }
    //     }
    // }
    record = objects[ind]->get_shape().intersect_w_ray(
         ray, objects[ind]->get_transform(), t);
    return record;
}

void World::set_bg(Color& bg){
    background_color = bg;
}

Color World::get_bg(){
    return background_color;
}

void World::set_light(int ind, double force){
    get_obj(ind).set_light(force);
    lights.push_back(ind);
}

int World::lights_number(){
    return lights.size();
}

Object& World::get_light(int num){
    return *objects[lights[num]].get();
}
