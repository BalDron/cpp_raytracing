struct IntersectionInfo{
    int ind;
    double t;
};

class Object{
private:
    int index;
    bool is_light;
    double light_force;
    Camera camera;
    Transform transform;
    Shape shape;
public:
    Object(int ind, Vector3& pos);
    Object(int ind);
    int get_ind();
    void set_light(double force);
    bool check_light();
    ~Object();
    Component& get_component(Component_name comp_name);
    Camera& get_camera();
    Shape& get_shape();
    Transform& get_transform();
    double get_light_force();
};

class World{
private:
    Color background_color;
    int index_count;
    int active_cam;
    vector<int> lights;
    vector<std::shared_ptr<Object>> objects;
public:
    World();
    ~World();
    int size() const;
    int new_obj();
    Object& get_obj(int ind);
    Object& operator[](int ind);
    void rem_obj(int ind);
    int find_camera();

    IntersectionInfo check_intersections(Ray& ray);
    HitRecord intersect(Ray& ray, double t, int ind);
    void set_bg(Color& bg);
    Color get_bg();
    void set_light(int ind, double force);
    int lights_number();
    Object& get_light(int num);
};
