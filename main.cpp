#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

#include "vector3.h"
#include "color.h"
#include "ray.h"
#include "component.h"
#include "object.h"
#include "raytracer.h"


template<typename T>
vector<T> read_from(ifstream& src, int num){
    vector<T> cont;
    T tmp;
    for (int i = 0; i < num; ++i){
        src >> tmp;
        cont.push_back(tmp);
    }
    return cont;
}

vector<int> read_camera_config(){
    ifstream file;
    file.open("build.camera");
    int x, y, depth;
    string tmp;
    file >> tmp;
    x = stoi(tmp);
    file >> tmp;
    y = stoi(tmp);
    file >> tmp;
    depth = stoi(tmp);
    file.close();
    return {x, y, depth};
}

void read_world_config(World& objs){
    ifstream w_file;
    w_file.open("build.world");
    string tmp;
    w_file >> tmp;
    int count = stoi(tmp);
    vector<double> coord;
    vector<double> clr;
    vector<double> mat;
    int new_ind;
    // reading objects
    for (int j = 0; j < count; ++j){
        cout << "====\n";
        coord.clear();
        coord = read_from<double>(w_file, 3);
        cout << "pos: " << coord[0] << " " << coord[1] << " " << coord[2] << endl;

        clr.clear();
        clr = read_from<double>(w_file, 3);
        cout << "clr: " << clr[0] << " " << clr [1] << " " << clr[2] << endl;

        mat.clear();
        mat = read_from<double>(w_file, 3);
        cout << "mat: " << mat[0] << " " << mat[1] << " " << mat[2] << endl;

        Color col{clr[0], clr[1], clr[2]};
        Vector3 vec{coord[0], coord[1], coord[2]};

        new_ind = objs.new_obj();
        cout << "ind: " << new_ind << endl;
        objs[new_ind].get_transform().set_pos(vec);
        objs[new_ind].get_material().set(mat[0], mat[1], mat[2]);

        string shape_type = read_from<string>(w_file, 1)[0];

        if (shape_type == "s") {
            double rad = read_from<double>(w_file, 1)[0];
            cout << "rad: " << rad << endl;
            objs[new_ind].get_shape().set_type(Shape_type::sphere);
            objs[new_ind].get_shape().set_color(col);
            objs[new_ind].get_shape().set_rad(rad);
        } else if (shape_type == "p"){
            vector<double> n = read_from<double>(w_file, 3);
            Vector3 normal{n[0],n[1],n[2]};
            cout << "norm: " << normal[0] << " " << normal[1] << " " << normal[2] << endl;
            normal.unite();
            objs[new_ind].get_shape().set_type(Shape_type::plane);
            objs[new_ind].get_shape().set_color(col);
            objs[new_ind].get_shape().add_param(normal);
        } else if (shape_type == "t"){
            vector<Vector3> pos;
            for (int k = 0; k < 3; ++k){
                coord.clear();
                coord = read_from<double>(w_file, 3);
                Vector3 vec{coord[0], coord[1], coord[2]};
                pos.push_back(vec);
                cout << "pos "<< k << ": " << coord[0] << " " << coord[1] << " " << coord[2] << endl;
            }
            objs[new_ind].get_shape().set_type(Shape_type::triangle);
            objs[new_ind].get_shape().set_color(col);
            objs[new_ind].get_shape().set_params(pos);
        }


    }

    // reading lights
    w_file >> tmp;
    count = stoi(tmp);
    for (int j = 0; j < count; ++j){
        coord.clear();
        coord = read_from<double>(w_file, 3);
        cout << "\n";
        w_file >> tmp;
        double force = stod(tmp);
        new_ind = objs.new_obj();
        Vector3 vec{coord[0], coord[1], coord[2]};
        objs[new_ind].get_transform().set_pos(vec);
        objs.set_light(new_ind, force);
    }

    // reading background
    clr.clear();
    for (int i = 0; i < 3; ++i){
        w_file >> tmp;
        cout << tmp << " ";
        clr.push_back(stod(tmp));
    }
    cout << endl;
    Color c{clr[0], clr[1], clr[2]};
    objs.set_bg(c);

    w_file.close();
}

void put_it_out(vector<vector<Color>>& pixels, string filename){
    ofstream file;
    file.open(filename);
    file << "P3 ";
    file << pixels[0].size() << " ";
    file << pixels.size() << " 255\n";

    for (int i = 0; i < pixels.size(); ++i){
        for (int j = 0; j < pixels[0].size(); ++j){
            Color tmp = pixels[i][j];
            file << int(tmp.r()) << " " << int(tmp.g()) << " " << int(tmp.b()) << " ";
        }
        file << "\n";
    }

    file.close();
}

int main(){
    vector<int> resolution;
    resolution = read_camera_config();
    World objs{};
    int new_ind = objs.new_obj();
    double screen_width = 3.0;
    double screen_height = screen_width * resolution[1] / resolution[0];

    objs[new_ind].get_camera().setup(
        Vector3(- (screen_width / 2), - (screen_height / 2), -1.0),
        Vector3(screen_width, 0.0, 0.0),
        Vector3(0.0, screen_height, 0.0)
    );
    objs[new_ind].get_camera().set_active();

    read_world_config(objs);

    Raytracer rt{resolution[0], resolution[1], resolution[2]};
    auto start = chrono::high_resolution_clock::now();
    rt.make_step(objs);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << duration.count() << " ms" << endl;
    put_it_out(rt.get_image(), "try1.bmp");
}
