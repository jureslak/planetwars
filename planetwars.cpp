#include "Settings.h"
#include "Objects.hpp"
#include "Functions.hpp"
#include "Physics.hpp"
#include "Drawing.hpp"

#include <iostream>
#include <vector>
#include <cstdio>
#include <cassert>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

using std::vector;
using std::cout;

int main(int argc, char* argv[]) { 

    assert( !init(SCREEN_WIDTH, SCREEN_HEIGHT) );
    image::load_images();

    vector<Level> levels;
    load_levels(levels);

    int fc = 0;
  
    Ball ball (0, SCREEN_HEIGHT/2);

    Target target (SCREEN_WIDTH-30, SCREEN_HEIGHT/2.5, 40, 150);
    Cannon cannon (0, SCREEN_HEIGHT/2);

    vector<Planet> planets;
    next_level(levels, planets, cannon, target, ball);
    vector<Planet>::iterator active = planets.end();

    bool done = false;
    bool playing = false;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type)
            {
                case SDL_QUIT:
                    done = true;
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    if (event.key.keysym.sym == SDLK_r) {
                        ball.reset();
                        playing = false;
                        active = planets.end();
                    }
                    if (event.key.keysym.sym == SDLK_SPACE)
                        if (!playing) {
                            cannon.fire(ball);
                            active = planets.end();
                            playing = true;
                        }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (!playing)
                        active = find_clicked(planets, event.button.x, event.button.y);
                    break;
                    
                case SDL_MOUSEBUTTONUP:
                    active = planets.end();
                    break;

                case SDL_MOUSEMOTION:
                    if (active != planets.end()) {
                        active->set_pos(event.motion.x, event.motion.y);
                    }                    
                    break;
            }
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        
        // Physics
        if (playing) {
            object_move(ball); 
            apply_gravity(ball, planets);
            if (hit_target_check(ball, target)) {
                playing = false;
                active = planets.end();
                if (!next_level(levels, planets, cannon, target, ball)) {
                    done = true;
                    cout << "You won!" << endl;
                }
            }
            apply_collision(ball, planets);
            if (!on_screen(SCREEN_WIDTH, SCREEN_HEIGHT, ball)) {
                ball.reset();
                playing = false;
                active = planets.end();
            }
        }

        // Drawing
        if (!playing)
            draw(cannon);
        draw(ball);
        draw(planets.begin(), planets.end());
        draw(target);

        SDL_GL_SwapBuffers();
        fc++;
    }
    cout << "Total frames:" <<  fc << endl;

    SDL_Quit();

    return 0;
}

