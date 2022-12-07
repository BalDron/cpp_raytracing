class Object{
private:
    vector<std::unique_ptr<Component>> components;
    int index;
public:
    Object(int ind);
    int get_ind();
    void set_light(double force);
    bool check_component(Component_name comp_name);
    void add_component(Component_name comp_name);
    Component& get_component(Component_name comp_name);
    Camera& get_camera();
    Shape& get_shape();
    Transform& get_transform();
    Material& get_material();
    Light& get_light();
    double get_light_force();
};

class World{
private:
    Color background_color;
    int index_count;
    int active_cam;
    vector<int> lights;
    vector<std::unique_ptr<Object>> objects;
public:
    World();
    ~World();
    int size() const;
    int new_obj();
    Object& get_obj(int ind);
    Object& operator[](int ind);
    void rem_obj(int ind);
    int find_camera();

    HitRecord& check_intersections(Ray& ray, HitRecord& record);
    HitRecord& intersect(Ray& ray, HitRecord& record);
    void set_bg(Color& bg);
    Color get_bg();
    void set_light(int ind, double force);
    int lights_number();
    Object& get_light(int num);
};
