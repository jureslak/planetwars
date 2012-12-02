#ifndef DRAWING_HEADER
#define DRAWING_HEADER

#include "Objects.hpp"
#include "Image.hpp"
#include <GL/gl.h>
#include <cmath>

namespace color {

static float ball_color[]   = {1, 0, 0, 1};
static float planet_color[] = {0.5, 0.5, 1, 0.3};
static float target_color[] = {0, 1, 0, 1};
static float cannon_color[] = {0.55, 0.91, 1, 1};
static float WHITE[] = {1, 1, 1, 1};

}

inline double radtodeg(const double& rad) {
    return rad/M_PI*180;
}
inline double degtorad(const double& deg) {
    return deg*M_PI/180;
}

void draw_circle(float cx, float cy, float r, int num_segments=60) 
{ 
    float theta = 2 * M_PI / num_segments; 
    float c = cosf(theta); // precalculate the sine and cosine
    float s = sinf(theta);
    float t;

    float x = 1, y = 0; // we start at angle = 0 
    
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.5, 0.5);
        glVertex2f(cx, cy);
        for(int ii = 0; ii <= num_segments; ii++) { 
            glTexCoord2f(0.5 + x/2, 0.5 + y/2);
            glVertex2f(x*r + cx, y*r + cy); // output vertex 

            t = x;                      // apply the rotation matrix of unit circle
            x = c * x - s * y;
            y = s * t + c * y;
        } 
    glEnd(); 
}

void draw_rect(int x, int y, int w, int h) {
    glBegin(GL_QUADS);
        glTexCoord2i(0, 1); glVertex2f(x,   y);   
        glTexCoord2i(1, 1); glVertex2f(x+w, y);   
        glTexCoord2i(1, 0); glVertex2f(x+w, y+h); 
        glTexCoord2i(0, 0); glVertex2f(x,   y+h); 
    glEnd();
}

void draw_line(const Vec& start, const Vec& end, const float width=2) {
    glPushMatrix();
    glTranslatef(start.x, start.y, 0);
    Vec relv = end - start;
    float phi = radtodeg(atan2(relv.y, relv.x));
    glRotatef(phi,0,0,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1); glVertex2f(0, -width);
        glTexCoord2f(0,0); glVertex2f(0, width);
        float abs = relv.size();
        glTexCoord2f(abs/2/width,0); glVertex2f(abs, width);
        glTexCoord2f(abs/2/width,1); glVertex2f(abs, -width);
    glEnd();
    glPopMatrix();
}

void draw_arrow(const Vec& start, const Vec& end, const float width=2, const float head=10, const float phi=30) {
    draw_line(start, end, width);
    // head line 1
    float c = cos(degtorad(phi)), s = sin(degtorad(phi));
    Vec hl1 = start - end, hl2;
    hl1.normalize();
    hl1 *= head;
    hl2 = hl1;
    hl1.matrix_multiply(c, -s, s, c);
    hl2.matrix_multiply(c, s, -s, c);
    hl1 += end;
    hl2 += end;
    draw_line(end, hl1, width);
    draw_line(end, hl2, width);
    draw_circle(start.x, start.y, width);
    draw_circle(end.x, end.y, width);
    draw_circle(hl1.x, hl1.y, width);
    draw_circle(hl2.x, hl2.y, width);
}

void draw(const Ball& b) {
    glColor4fv(color::ball_color);
    glColor4fv(color::WHITE);
    glPushMatrix();
    glTranslatef(b.x(), b.y(), 0.1);
    glRotatef(radtodeg(atan2(b.speed.y,b.speed.x)),0,0,1);
    image::ball_image->activate();
    draw_circle(0, 0, b.r());
    image::ball_image->deactivate();
    glPopMatrix();
}

void draw(const Planet& p) {
    glPushMatrix();
    glColor4fv(color::planet_color);
    glColor4fv(color::WHITE);
    image::atm_image->activate();
    draw_circle(p.x(), p.y(), p.r());
    image::atm_image->deactivate();
    glColor4fv(color::WHITE);
    image::planet_images[p.type]->activate();
    draw_circle(p.x(), p.y(), p.inr());
    image::planet_images[p.type]->deactivate();
    glPopMatrix();
}

void draw(const Target& t) {
    glPushMatrix();
    glColor4fv(color::target_color);
    glColor4fv(color::WHITE);
    image::target_image->activate();
    draw_rect(t.x(), t.y(), t.w(), t.h());
    image::target_image->deactivate();
    glPopMatrix();
}

void draw(const Cannon& c) {
//    image::target_image->activate();
//    draw_rect(c.start.x-20, c.start.y-75, 40, 150);
//    image::target_image->deactivate();
    glColor4fv(color::cannon_color);
    Vec e = c.start + (c.end - c.start)*25;
    draw_arrow(c.start, e, 1);
}

template <typename RAI>
void draw(RAI b, RAI e) {
    while (b != e) draw(*b++);
}

#endif
