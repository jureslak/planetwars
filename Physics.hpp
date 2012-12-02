#ifndef PHYSICS_HEADER
#define PHYSICS HEADER

#include "Objects.hpp"
#include "Functions.hpp"
#include <vector>
#include <iostream>
#include <algorithm>

using std::vector;
using std::cout;
using std::endl;
using std::max;

const static float air_resistance_acceleration = 0.000;
const static float planet_collision_absorbtion_coefficient = 0.3; // 30% of energy lost in collision

template <typename T>
void object_move(T& t) {
    t.pos += t.speed;
    t.speed *= (1 - air_resistance_acceleration);
}

void apply_gravity(Ball& b, const vector<Planet>& planets) {
    for (size_t i = 0; i < planets.size(); ++i) {
        const Planet& p = planets[i];
        if (inside_circ(b, p)) { // ball inside of planet
            Vec dir = p.pos - b.pos;
            int size = dir.size()/6;
            dir.normalize();
            dir *= p.g()/(size*size);
            b.speed += dir;
        }
    }
}

bool hit_target_check(Ball &b, const Target& t) {
    return inside_rect(b, t);
}

void apply_collision(Ball& b, const vector<Planet>& planets) {
    for (size_t i = 0; i < planets.size(); ++i) {
        const Planet& p = planets[i];
        if (collide_circ(b, p)) {
            Vec g = b.pos - p.pos;
            g.normalize();
            b.speed += g*abs(g.projection(b.speed))*2*(1-planet_collision_absorbtion_coefficient);
            b.pos = p.pos + g*(b.r() + p.inr());
            break;
        }
    }
}

#endif
