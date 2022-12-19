#ifndef __COMPONENT__
#define __COMPONENT__

#include <memory>
using std::vector;

class Vector3;
class Component;

enum class Component_name{
    transform,
    camera,
    shape,
    material,
    light
};

enum class Shape_type{
    sphere,
    plane,
    triangle
};

struct HitRecord{
    double t;
    int ind;
    Vector3 hit_point;
    Vector3 hit_normal;
    Color color;
};

class Component{
public:
    virtual void set_name(Component_name n) = 0;
    virtual Component_name get_name() const = 0;
    virtual bool check_name(Component_name n) const = 0;

};

class NamedComponent : public Component{
private:
    Component_name name;
public:
    void set_name(Component_name n) override;
    Component_name get_name() const override;
    bool check_name(Component_name n) const override;
};

class Transform: public NamedComponent{
private:
    Vector3 position;
    Vector3 rotation;
public:
    Transform();
    Transform(Vector3& pos);
    Vector3& pos();
    Vector3& rot();
    void set_pos(const Vector3& new_pos);
};

class Camera: public NamedComponent{
private:
    bool is_active;
    Vector3 ll_corner;
    Vector3 horisontal;
    Vector3 vertical;
public:
    Camera();
    Camera( const Vector3 &ll_corn,
            const Vector3 &hor,
            const Vector3 &vert);
    Ray get_ray(double nx, double ny, Transform& transform);
    void set(bool stat);
    void set_active();
    void set_inactive();
    void setup(
            const Vector3 &ll_corn,
            const Vector3 &hor,
            const Vector3 &vert);
    bool check_active();
};

class Shape: public NamedComponent{
private:
    Color color = Color(0, 0, 0);
    Shape_type type;
    double sphere_radius = 0.0;
    vector<Vector3> params;

    vector<double> sphere_count_abcd(Ray& ray, Vector3& pos);
public:
    Shape();
    Shape(Shape_type t);
    Shape(Color col, double rad); // sphere
    Shape(Color col, Vector3 n); // plane
    Shape(Color col, vector<Vector3> local_coords); // triangle
    ~Shape();
    void set_type(Shape_type t);
    void set_color(Color& col);
    void set_rad(double rad);
    void set_params(vector<Vector3>& p);
    void add_param(Vector3& p);
    void clear_params();
    Color& get_color();
    Vector3 get_normal(const HitRecord& rec, Transform& transform);
    double check_triangle_intersection(Ray& ray, Transform& transform);
    double check_plane_intersection(Ray& ray, Transform& transform);
    double check_sphere_intersection(Ray& ray, Transform& transform);
    double check_intersection(Ray& ray, Transform& transform);
    HitRecord& intersect_w_ray(Ray& ray, Transform& transform, HitRecord& record);
};


class Material: public NamedComponent{
private:
    double self;
    double mirror;
    double glass;
public:
    Material();
    Material(double s, double m);
    double get_self();
    double get_mirror();
    double get_glass();
    void set(double s, double m, double g);
};

class Light: public NamedComponent {
private:
    double force = 0;
public:
    Light();
    Light(double f);
    double get_force();
    void set_force(double f);
};

#endif
