#include "common.h"
#ifndef __SDL_FUNC_H__
#define __SDL_FUNC_H__
SDL_Window* sdl_getwindow();
bool sdl_init();
void sdl_close(int sig);
void sdl_free();
void sdl_frame();
char * sdl_pixels_lock();
void sdl_pixels_unlock();
#endif //__SDL_FUNC_H__