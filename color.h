class Color{
private:
    vector<int> rgb;
public:
    Color();
    Color(int r, int g, int b);
    void set(int r, int g, int b);

    void check_bound();

    int r();
    int g();
    int b();
    int operator[](int i);

    Color operator+(Color col);
    Color operator-(Color col);
    Color operator*(int mul);
    Color operator/(int mul);
};
