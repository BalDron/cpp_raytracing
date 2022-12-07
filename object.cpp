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

bool Object::check_component(Component_name comp_name){
    for (long unsigned int i = 0; i < components.size(); ++i){
        if (components[i]->get_name() == comp_name){
            return true;
        }
    }
    return false;
}

Component& Object::get_component(Component_name comp_name){
    for (long unsigned int i = 0; i < components.size(); ++i){
        if (components[i]->get_name() == comp_name){
            return *components[i];
        }
    }
    std::string nm;
    switch (comp_name) {
        case Component_name::transform:
            nm = "transform";
            break;
        case Component_name::camera:
            nm = "camera";
            break;
        case Component_name::shape:
            nm = "shape";
            break;
        case Component_name::material:
            nm = "material";
            break;
        default:
            throw std::runtime_error(
                "Object.add_component: unknown component name"
            );
    }
    throw std::runtime_error(
        "Object.get_component: no such component: " + nm
    );
}

void Object::add_component(Component_name comp_name){
    if (check_component(comp_name)){
        throw std::runtime_error(
            "Object.add_component: such component is already added."
        );
    }
    std::unique_ptr<Component> cmp;
    switch (comp_name) {
        case Component_name::transform:
            components.push_back(std::make_unique<Transform>());
            break;
        case Component_name::camera:
            components.push_back(std::make_unique<Camera>());
            break;
        case Component_name::shape:
            components.push_back(std::make_unique<Shape>());
            break;
        case Component_name::material:
            components.push_back(std::make_unique<Material>());
            break;
        default:
            throw std::runtime_error(
                "Object.add_component: unknown component name"
            );
    }
}

Camera& Object::get_camera(){
    Camera& camera = dynamic_cast<Camera&>(
        get_component(Component_name::camera)
    );
    return camera;
}

Shape& Object::get_shape(){
    Shape& shape = dynamic_cast<Shape&>(
        get_component(Component_name::shape)
    );
    return shape;
}

Transform& Object::get_transform(){
    Transform& transform = dynamic_cast<Transform&>(
        get_component(Component_name::transform)
    );
    return transform;
}

Material& Object::get_material(){
    Material& material = dynamic_cast<Material&>(
        get_component(Component_name::material)
    );
    return material;
}

Object::Object(int ind){
    index = ind;
}

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

World::World():
    index_count{0}
{}

World::~World(){
    lights.clear();
}

int World::size() const{
    return objects.size();
}

int World::new_obj(){
    objects.push_back(std::make_unique<Object>(index_count));
    index_count += 1;
    return index_count - 1;
}

Object& World::get_obj(int ind){
    if (ind < 0 || ind >= size()){
        throw std::runtime_error(
            "World.get_obj(): ind is out of size"
        );
    }
    for (long unsigned int i = 0; i < objects.size(); ++i){
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
    for (long unsigned int i = 0; i < objects.size(); ++i){
        if (objects[i]->get_ind() == ind){
            return *objects[i].get();
        }
    }
    throw std::runtime_error(
        "World.operator[](): object requested not found"
    );
}

int World::find_camera(){
    for (long unsigned int i = 0; i < objects.size(); ++i){
        if (objects[i]->check_component(Component_name::camera)
                && objects[i]->get_camera().check_active()){
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

HitRecord& World::check_intersections(Ray& ray, HitRecord& record) {
    double tmp_t;
    record.t = -1.0;
    for (int i = 0; i < size(); ++i){
        if (objects[i]->check_component(Component_name::shape)) {
            tmp_t = objects[i]->get_shape().check_intersection(ray, objects[i]->get_transform());
            if (tmp_t > 0.0 && (tmp_t < record.t || record.t <= 0.0)){
                record.t = tmp_t;
                record.ind = objects[i]->get_ind();
            }
        }
    }
    return record;
}

HitRecord& World::intersect(Ray& ray, HitRecord& record){
    record = objects[record.ind]->get_shape().intersect_w_ray(
         ray, objects[record.ind]->get_transform(), record);
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
