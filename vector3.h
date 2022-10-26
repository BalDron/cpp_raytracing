#include <vector>

class Vector3{
private:
    std::vector<double> axis;
    // static constexpr int dim = 3;
public:
    Vector3(double a, double b, double c);

    void set(double a, double b, double c);
    void set_x(double a);
    void set_y(double b);
    void set_z(double c);
    void set(int i, double x);

    double x();
    double y();
    double z();
    double operator[](int i);

    Vector3 operator+(Vector3& vec);
    Vector3 operator-(Vector3& vec);
    Vector3 operator-();
    Vector3 operator*(double mul);
    Vector3 operator/(double mul);

    Vector3& operator+=(Vector3 vec);
    Vector3& operator-=(Vector3 vec);
    Vector3& operator*=(double mul);
    Vector3& operator/=(double mul);

    double len();
    Vector3 unite();
};
