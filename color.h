class Color{
private:
    vector<int> rgb;
public:
    Color();
    Color(int r, int g, int b);
    Color(const Color& col);
    void set(int r, int g, int b);

    void check_bound();

    int r() const;
    int g() const;
    int b() const;
    int operator[](int i) const;

    Color operator+(Color& col);
    Color operator-(Color& col);
    Color operator*(int mul);
    Color operator/(int mul);

    Color& operator=(const Color& col);
};
