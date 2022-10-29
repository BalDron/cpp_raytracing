class Object{
private:
    int index;
    Camera camera;
    Transform transform;
    Shape shape;
public:
    Object(int ind, Vector3& pos);
    Object(int ind);
    int get_ind();
    ~Object();
    Component& get_component(Component_name comp_name);
    Camera& get_camera();
    Shape& get_shape();
    Transform& get_transform();
};

class World{
private:
    Color background_color = Color(15, 15, 100);
    int index_count;
    int active_cam;
    vector<std::shared_ptr<Object>> objects;
public:
    World();
    // ~World();
    int size() const;
    int new_obj();
    Object& get_obj(int ind);
    Object& operator[](int ind);
    void rem_obj(int ind);
    int find_camera();

    bool check_intersections(Ray& ray);
    HitRecord intersect(Ray& ray);
    Color get_bg();
};
