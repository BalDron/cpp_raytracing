class Raytracer{
private:
    int resolution_x;
    int resolution_y;
    vector<vector<Color>> colors;
public:
    Raytracer(int res_x, int rex_y);
    int make_step(World& objs);
    Color cast_ray(Ray& ray, World& objs, int depth);
    vector<vector<Color>>& get_image();
};
