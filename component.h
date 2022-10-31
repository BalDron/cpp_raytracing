enum class Component_name{
    transform,
    camera,
    shape,
};

enum class Shape_type{
    circle,
};

struct HitRecord{
    double t;
    int ind;
    Vector3 hit_point;
    Vector3 hit_normal;
    Color color;
};

class Component{
private:
    Component_name name;

public:
    Component(Component_name n);
    Component_name get_name() const;
    bool check_name(Component_name n);
};

class Transform: public Component{
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

class Camera: public Component{
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

class Shape: public Component{
private:
    Color color;
    Shape_type type;
    double sphere_radius;

    vector<double> sphere_count_abcd(Ray& ray, Vector3& pos);
public:
    Shape();
    Shape(Color color, double rad);
    void set_color(Color& col);
    void set_rad(double rad);
    Color& get_color();
    double check_intersection(Ray& ray, Transform& transform);
    HitRecord& intersect_w_ray(Ray& ray, Transform& transform, HitRecord& record);
};
