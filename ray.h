class Ray{
private:
    Vector3 origin;
    Vector3 direction;
public:
    Ray(const Vector3& orig, const Vector3& dir);
    Vector3 at(double t);
    void set_origin(const Vector3& new_orig);
    void set_direction(const Vector3& new_dir);
    Vector3& get_origin();
    Vector3& get_direction();
};
