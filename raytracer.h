#include <vector>
#include "vector3.h"
#include "color.h"

using std::vector;

class Ray{
private:
    Vector3 origin;
    Vector3 direction;
public:
    Ray(const Vector3& orig, const Vector3& dir);
    Vector3 at(double t);
    void set_origin(const Vector3& new_orig);
    void set_direction(const Vector3& new_dir);
    Vector3 set_origin();
    Vector3 set_direction();
};

class Raytracer{
private:
    int rex_x;
    int res_y;
    vector<vector<Color>> colors;
    int find_camera();
public:
    Raytracer();
    vector<vector<Color>>& make_step(
            const vector<Object>& objs,
            const vector<vector<Color>>& colors);
};
