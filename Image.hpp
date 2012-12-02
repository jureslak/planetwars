#ifndef IMAGE_HEADER
#define IMAGE_HEADER

#include "Objects.hpp"
#include "Functions.hpp"
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include <iostream>
#include <string>

using std::cerr;
using std::string;

namespace image { 

class Image {
    SDL_Surface* surface;
    GLuint texture;
    GLenum texture_format;
    GLint color_num;
    unsigned width, height;
    float aspect_ratio;

public:    

    Image(const char* filename) { load_image(filename); }
    Image(const string& filename) { load_image(filename.c_str()); }


    void load_image(const char* filename) {
        surface = IMG_Load( filename ); /* < load the texture */
        if (surface == NULL) {
            cerr << "[error] Image loading failed for image " << filename << " with error: " << IMG_GetError() << endl;
        }

        width = surface->w;
        height = surface->h;
        aspect_ratio = width / float(height);
        color_num = surface->format->BytesPerPixel;

        if (!((width & (width - 1)) == 0 && (height & (height - 1)) == 0)) {
            cerr << "[warning] Size of image " << filename << " not a power of 2" << endl;
        }

        if (color_num == 4) { // RGBA
            texture_format = (surface->format->Rmask == 0xff) ? GL_RGBA : GL_BGRA;
        } else if (color_num == 3) { // RGB
            texture_format = (surface->format->Rmask == 0xff) ? GL_RGBA : GL_BGRA;
        } else {
            cerr << "[warning] Image " << filename << " is not of any known format." << endl;
        }
        
        glGenTextures(1, &texture); /* generate a name for our texture */
        if (texture == 0) {
            cerr << "[error] Name generation failed for image " << filename  << 
                " with error: " << glGetError() << endl;
        }

        glBindTexture( GL_TEXTURE_2D, texture);

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
       
        glTexImage2D( GL_TEXTURE_2D, 0, color_num, width, height, 0,
                      texture_format, GL_UNSIGNED_BYTE, surface->pixels ); 

        SDL_FreeSurface( surface );

    } // end load image

    void activate() {
        glBindTexture( GL_TEXTURE_2D, texture );
    }

    void deactivate() {
        glBindTexture( GL_TEXTURE_2D, 0 );
    }

    unsigned w() { return width; }
    unsigned h() { return height; }
    float ratio() { return aspect_ratio; }
};

Image* ball_image;
Image* atm_image;
Image* target_image;
vector<Image*> planet_images;

void load_images() {
    ball_image = new Image("image/ball.png");
    atm_image = new Image("image/atmosphere.png");
    target_image = new Image("image/target.png");
    load_planets_spec();
    map<string,Planet>::const_iterator it = planet::planets.begin();
    for (; it != planet::planets.end(); ++it)
         planet_images.push_back(new Image("image/planets/"+it->first+".png"));
}

} // end namespace

#endif
