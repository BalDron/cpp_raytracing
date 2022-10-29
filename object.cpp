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

Object::~Object()
{}

World::World():
    index_count{0}
{}

int World::size() const{
    return objects.size();
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

bool World::check_intersections(Ray& ray){
    for (int i = 0; i < size(); ++i){
        if (objects[i]->get_shape().check_intersection(ray, objects[i]->get_transform())
                && !objects[i]->get_camera().check_active()){
            // std::cout << i << "\n";
            return true;
        }
    }
    return false;
}

HitRecord World::intersect(Ray& ray){
    HitRecord record, temp;
    bool record_empty = true;
    for (int i = 0; i < size(); ++i){
        if (!objects[i]->get_camera().check_active()){
            if (objects[i]->get_shape().check_intersection(
                    ray, objects[i]->get_transform())){
                temp = objects[i]->get_shape().intersect_w_ray(
                        ray, objects[i]->get_transform());
                if (record_empty || temp.t <= record.t){
                    record = temp;
                    record_empty = false;
                }
            }
        }
    }
    return record;
}

Color World::get_bg(){
    return background_color;
}
