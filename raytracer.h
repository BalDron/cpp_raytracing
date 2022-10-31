class Raytracer{
private:
    int resolution_x;
    int resolution_y;
    int ray_depth;
    vector<vector<Color>> colors;
public:
    Raytracer(int res_x, int rex_y, int depth);
    int make_step(World& objs);
    Color cast_ray(Ray& ray, World& objs, int depth, HitRecord& prev_rec);
    Vector3 reflect(Vector3& ray_dir, Vector3& normal);
    vector<vector<Color>>& get_image();
};
