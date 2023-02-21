#ifndef __COLOR__
#define __COLOR__

class Color{
private:
    vector<double> rgb;
public:
    Color();
    Color(double r, double g, double b);
    Color(double c);
    Color(const Color& col);
    ~Color();
    void set(double r, double g, double b);

    void check_bound();

    double r() const;
    double g() const;
    double b() const;
    double operator[](int i) const;
    void lighten(double lr, double lg, double lb);
    void lighten(double l);
    void lighten(Color l);

    Color operator+(Color col);
    Color operator-(Color col);
    Color operator*(double mul);
    Color operator/(double mul);

    Color& operator=(const Color& col);
};

#endif
