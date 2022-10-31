class Vector3{
private:
    vector<double> axis;
public:
    Vector3(double a, double b, double c);

    Vector3(const Vector3& vec); // copy constructor
    Vector3& operator=(const Vector3& vec); // copy assignment

    Vector3(const Vector3&& vec); //move constructor
    Vector3& operator=(Vector3&& vec); // move assignment

    Vector3();
    ~Vector3();

    void set(double a, double b, double c);
    void set_x(double a);
    void set_y(double b);
    void set_z(double c);
    void set(int i, double x);

    double x() const;
    double y() const;
    double z() const;
    double operator[](int i) const;

    Vector3 operator+(const Vector3& vec);
    Vector3 operator-(const Vector3& vec);
    Vector3 operator-();
    Vector3 operator*(double mul);
    Vector3 operator/(double mul);

    bool operator!=(const Vector3 vec);

    // Vector3& operator+=(Vector3 vec);
    // Vector3& operator-=(Vector3 vec);
    // Vector3& operator*=(double mul);
    // Vector3& operator/=(double mul);

    double len();
    void unite();
    void print();
    void clean();
};


double dot(Vector3& vec1, Vector3& vec2);

Vector3 unit_vector(Vector3& vec);
