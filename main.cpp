#include <iostream>
#include "common.h"
#include "geometry.h"
#include "constants.h"
#include "sdl_funcs.h"
#include "objects.h"
#include "raytrace.h"
#include "vector.h"
SDL_Event event;
vec3 rotate(vec3 v, const vec3 k)
{
    double cos_theta = cos(k.x);
    double sin_theta = sin(k.x);
    v = (v * cos_theta) + (vec3(1,0,0).cross(v) * sin_theta) + (vec3(1,0,0) * vec3(1,0,0).dot(v)) * (1 - cos_theta);


    cos_theta = cos(k.y);
    sin_theta = sin(k.y);
    v = (v * cos_theta) + (vec3(0,1,0).cross(v) * sin_theta) + (vec3(0,1,0) * vec3(0,1,0).dot(v)) * (1 - cos_theta);
    
    cos_theta = cos(k.z);
    sin_theta = sin(k.z);
    v = (v * cos_theta) + (vec3(0,0,1).cross(v) * sin_theta) + (vec3(0,0,1) * vec3(0,0,1).dot(v)) * (1 - cos_theta);

    return v;
}
void render(const std::vector<Sphere> &spheres, const std::vector<Light> &lights,const Cam &cam) {
    const float fov  = cam.fov;
    std::vector<color> framebuffer(SCREEN_WIDTH*SCREEN_HEIGHT);


    #pragma omp parallel for
    for (size_t j = 0; j<SCREEN_HEIGHT; j++) { // actual rendering loop
        for (size_t i = 0; i<SCREEN_WIDTH; i++) {
            float dir_x =  (i + 0.5) -  SCREEN_WIDTH/2.;
            float dir_y = -(j + 0.5) + SCREEN_HEIGHT/2.;    // this flips the image at the same time
            float dir_z = -SCREEN_HEIGHT/(2.*tan(fov/2.));
            framebuffer[i+j*SCREEN_WIDTH] = cast_ray(cam.pos, rotate(vec3{dir_x, dir_y, dir_z},cam.dir).normalize(), spheres, lights);
        }
    }
    size_t index = 0;
    char  *texture_pixels = sdl_pixels_lock();
    for (color &c : framebuffer) {

        c.rgb(texture_pixels+index);
        index += 3;
    }
    sdl_pixels_unlock();


    
}


void signal_hand(int signum) {
   std::cout << "Caught signal " << signum << std::endl;
   sdl_close(0);
}

int main() {
    int winsizeX, winsizeY;
    signal(SIGINT, signal_hand);
	if( !sdl_init() )
	{
		printf( "Failed to initialize!\n" );
        exit(-1);
	}

    const Material      ivory = {1.0, {0.6,  0.3, 0.1, 0.0}, {0.4, 0.4, 0.3},   50.};
    const Material      glass = {1.5, {0.0,  0.5, 0.1, 0.8}, {0.6, 0.7, 0.8},  125.};
    const Material red_rubber = {1.0, {0.9,  0.1, 0.0, 0.0}, {0.3, 0.1, 0.1},   10.};
    const Material     mirror = {1.0, {0.0, 10.0, 0.8, 0.0}, {1.0, 1.0, 1.0}, 1425.};

    std::vector<Sphere> spheres = {
        Sphere{vec3{-3,    0,   -16}, 2,      ivory},
        Sphere{vec3{-1.0, -1.5, -12}, 2,      glass},
        Sphere{vec3{ 1.5, -0.5, -18}, 3, red_rubber},
        Sphere{vec3{ 7,    5,   -18}, 4,     mirror}
    };

    std::vector<Light> lights = {
        {{-20, 20,  20}, {1.5,1,1}},
        {{ 30, 50, -25}, {1,1.8,1}},
        {{ 30, 20,  30}, {1,1,1.7}}
    };

    Cam cam = {
        {1,1,0},
        {1,1,0},
        M_PI_2
    };

    int xM = 0,yM = 0;
    while(1){

        SDL_PumpEvents();
        const Uint8 *keystates = SDL_GetKeyboardState(NULL);
        while( SDL_PollEvent( &event ) )
        {      
            if( event.type == SDL_QUIT )
                sdl_close(0);
            if(event.type == SDL_MOUSEMOTION){
                //SDL_GetMouseState(&,&yM);
                xM += event.motion.xrel;
                yM += event.motion.yrel;
                SDL_GetWindowSize(sdl_getwindow(), &winsizeX,&winsizeY);
                cam.dir = vec3{-(yM*M_PI/winsizeX)*1.,-(xM*M_PI/winsizeY)*1,0};
            }
        }
        if(keystates[SDL_SCANCODE_ESCAPE])
            signal_hand(0);
        if(keystates[SDL_SCANCODE_W])
            cam.pos -= rotate(vec3{0,0,1},vec3{-(yM*M_PI/winsizeX)*1.,-(xM*M_PI/winsizeY)*1.,0});
        if(keystates[SDL_SCANCODE_S])
            cam.pos += rotate(vec3{0,0,1},vec3{-(yM*M_PI/winsizeX)*1.,-(xM*M_PI/winsizeY)*1.,0});
        if(keystates[SDL_SCANCODE_A])
            cam.pos -= rotate(vec3{1,0,0},vec3{-(yM*M_PI/winsizeX)*1.,-(xM*M_PI/winsizeY)*1.,0});
        if(keystates[SDL_SCANCODE_D])
            cam.pos += rotate(vec3{1,0,0},vec3{-(yM*M_PI/winsizeX)*1.,-(xM*M_PI/winsizeY)*1.,0});
        if(keystates[SDL_SCANCODE_SPACE])
            cam.pos.y++;
        if(keystates[SDL_SCANCODE_LSHIFT])
            cam.pos.y--;
        render(spheres, lights,cam);
        sdl_frame();
    }
    sdl_free();
    return -2; // We shouldn't get here
}