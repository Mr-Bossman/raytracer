#include "common.h"
#include "constants.h"
#include <SDL2/SDL_ttf.h>
#ifndef __SDL_FUNC_H__
#define __SDL_FUNC_H__
SDL_Window* sdl_getwindow();
bool sdl_init();
void sdl_close(int sig);
void sdl_free();
void sdl_frame();
char * sdl_pixels_lock();
void sdl_pixels_unlock();

struct rgb {
    char r;
    char g;
    char b;
};
class frame32{
    public:
    size_t height;
    size_t width;
    uint32_t *framebuffer;
    frame32(size_t height_, size_t width_){
        framebuffer = (uint32_t*)sdl_pixels_lock();
        height = height_;
        width = width_;
    }
    ~frame32(){
        sdl_pixels_unlock();
    }
    uint32_t* operator[](const size_t h) {return framebuffer+h*width; }
    const uint32_t* operator[](const size_t h) const {return framebuffer+h*width; }
};
class frame{
    public:
    size_t height;
    size_t width;
    rgb *framebuffer;
    frame(size_t height_, size_t width_){
        framebuffer = (rgb*)sdl_pixels_lock();
        height = height_;
        width = width_;
    }
    ~frame(){
        sdl_pixels_unlock();
    }
    rgb* operator[](const size_t h) {return framebuffer+h*width; }
    const rgb* operator[](const size_t h) const {return framebuffer+h*width; }
};
void sdl_text(char * str);
#endif //__SDL_FUNC_H__