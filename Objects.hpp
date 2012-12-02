#ifndef OBJECTS_HEADER
#define OBJECTS_HEADER

#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using std::ostream;
using std::endl;
using std::cout;
using std::vector;
using std::string;

struct Vec {
    double x, y;
    Vec() {};
    Vec(double x_, double y_) : x(x_), y(y_) {};
    Vec operator+(const Vec& v) const {
        return Vec(x+v.x, y+v.y);
    }
    Vec& operator+=(const Vec& v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    Vec operator-(const Vec& v) const {
        return Vec(x-v.x, y-v.y);
    }
    Vec& operator-=(const Vec& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    Vec operator*(const double t) const {
        return Vec(x*t, y*t);
    }
    Vec& operator*=(const double t) {
        x *= t; 
        y *= t;
        return *this;
    }
    double operator*(const Vec& v) {
        return x*v.x + y*v.y;
    }
    double projection(const Vec& v) {
        return *this*v/size();
    }
    double norm() const {
        return x*x+y*y;
    }
    double size() const {
        return sqrt(norm());
    }
    double abs() const {
        return size();
    }
    void normalize() { // normalizes vector, i.e, after this, it has length 1
        double s = size();
        x /= s;
        y /= s;
    }
    void matrix_multiply(double a11, double a12, double a21, double a22) {
        double t = x;
        x = a11 * t + a12 * y;
        y = a21 * t + a22 * y;
    }
    bool operator==(const Vec& v) const { return x == v.x && y == v.y; }
    bool operator!=(const Vec& v) const { return x != v.x || y != v.y; }
//    friend ostream& operator<<(ostream& o, const Vec& v);
};
ostream& operator<<(ostream& o, const Vec& v) {
    o << '(' << v.x << ',' << v.y << ')'; return o;
}

double d(const Vec& a, const Vec& b) {
    return (a-b).size();
}

class Planet {
public:
    Vec pos;
    int type, radius, inner_radius;
    double gravity;
    Planet() {}
    Planet(int t, int x, int y, int r_, int inr, double g_) : pos(x,y), type(t), radius(r_), inner_radius(inr), gravity(g_) {};
    int x() const { return pos.x; }
    int y() const { return pos.y; }
    int r() const { return radius; }
    int inr() const { return inner_radius; }
    double g() const { return gravity; }
    void set_pos(int x, int y) { pos = Vec(x,y); }
};

struct Level {
    vector<string> planets;
    float r, phi;
    int start_height, end_height, end_size;
    void add(const string& s) {
        planets.push_back(s);
    }
};

class Ball {
public:
    Vec pos, speed, orig_pos;
    static const int r_ = 10;
    Ball(int x, int y) : pos(x,y), speed(0,0) {};
    int x() const { return pos.x; }
    int y() const { return pos.y; }
    int r() const { return r_; }
    void set(const Level& l) {
        orig_pos = Vec(0, l.start_height);
        pos = orig_pos;
    }
    void reset() {
        speed = Vec(0,0);
        pos = orig_pos;
    }
};

class Target {
private:
    int x_, y_, w_, h_;
public:
    Target(int _x, int _y, int _w, int _h) : x_(_x), y_(_y), w_(_w), h_(_h) {};
    int x() const { return x_; }
    int y() const { return y_; }
    int w() const { return w_; }
    int h() const { return h_; }
    void set(const Level& l) {
        y_ = l.end_height - l.end_size/2;
        h_ = l.end_size;
    }
};

class Cannon {
public:    
    Vec start, end;
    Cannon(int x, int y) : start(x,y), end() {};
    void set(float r, float phi, int sh) {
        start.y = sh;
        end = start+Vec(r*cos(phi),r*sin(phi)); 
    }
    void set(const Level& l) {
        set(l.r, l.phi, l.start_height);
    }
    void fire(Ball& b) {
        b.speed = end - start;
    }
};


#endif
