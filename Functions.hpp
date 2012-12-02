#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "Objects.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <utility>
#include <vector>

using std::ifstream;
using std::string;
using std::map;
using std::pair;
using std::vector;
using std::stringstream;

template <typename T, typename U>
bool inside_circ(const T& t, const U& u) {
    return d(t.pos, u.pos) < u.r();
}

template <typename T>
bool collide_circ(const T& t, const Planet& p) {
    return d(t.pos, p.pos) <= p.inr()+t.r();
}

template <typename T, typename U>
bool inside_rect(const T& t, const U& u) {
    return u.x() <= t.x() && t.x() <= u.x() + u.w() &&
           u.y() <= t.y() && t.y() <= u.y() + u.h();
}

template <typename T>
bool on_screen(int w, int h, const T& o) {
    return 0 <= o.pos.x && o.pos.x <= w &&
           0 <= o.pos.y && o.pos.y <= h;
}

namespace planet {
    map<string,Planet> planets;
}

using planet::planets;

void load_planets_spec() {
    if (planets.size() > 0) return;

    ifstream in ("spec/planets.spec");
    string name;
    int inr, r;
    float g;

    // reading planet spec
    in >> name >> inr >> r >> g;
    while (in) {
        planets[name] = Planet(0, 0, 0, r, inr, g);
        in >> name >> inr >> r >> g;
    }
    in.close();
    
    // generating types
    map<string,Planet>::iterator it = planets.begin();
    for (int i = 0; it != planets.end(); ++it, ++i) {
        it->second.type = i;
    }
}

void load_levels(vector<Level>& levels) {
    load_planets_spec();

    // reding level (s), each level - three lines
    string line, name;
    ifstream lin ("spec/level.spec");

    getline(lin, line);
    while (lin) {
        Level l;
        stringstream ss (line);
        ss >> name;
        while (ss) {
            l.add(name);
            ss >> name;
        } 
        getline(lin, line);
        stringstream sss(line);
        sss >> l.r >> l.phi >> l.start_height;
        getline(lin, line);
        stringstream ssss(line);
        ssss >> l.end_height >> l.end_size; 
        l.phi = -M_PI*l.phi/180;
        levels.push_back(l);
        getline(lin, line);
    }
}

bool next_level(const vector<Level>& levels, vector<Planet>& p, 
        Cannon& c, Target& t, Ball& b) {
    static unsigned int level_num = 0;
    if (level_num >= levels.size()) return false;

    p.clear();
    for (size_t i = 0; i < levels[level_num].planets.size(); ++i) {
        const string& name = levels[level_num].planets[i];
        p.push_back(Planet(planets[name].type, (i+1)*100, 150, planets[name].r(), planets[name].inr(), planets[name].g()));
    }
    c.set(levels[level_num]);
    t.set(levels[level_num]);
    b.set(levels[level_num]);
    level_num++;
    return true;
}

vector<Planet>::iterator find_clicked(vector<Planet>& p, int x, int y) {
    vector<Planet>::iterator it = p.end()-1;
    for (; it >= p.begin(); --it)
        if (d(Vec(x,y), it->pos) < it->inr())
            return it;
    return p.end();
}


#endif
