#include <iostream>
#include "common.h"
#include "geometry.h"
#include "constants.h"
#include "sdl_funcs.h"
#include "objects.h"
#include "raytrace.h"
#include "vector.h"
SDL_Event event;
struct rgb {
    char r;
    char g;
    char b;
};
rgb colorC(const color c) {
    rgb colors;
    char * arr = (char*)&colors;
    c.rgb(arr);
    return colors;
}
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

double alias[2] = {.25,.5};
int threshold[2] = {30,10}; // corners sides
inline bool checkT(rgb ca, rgb cb,int t){
    return ((abs(((int)ca.r)-((int)cb.r)) +abs(((int)ca.g)-((int)cb.g)) + abs(((int)ca.b)-((int)cb.b)) > t*3));
}
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

void render(const Objects &spheres, const Lights &lights,const Cam &cam) {
    const float fov  = cam.fov;
    rgb frame1[SCREEN_HEIGHT+2][SCREEN_WIDTH+2];
    frame framebuffer(SCREEN_HEIGHT,SCREEN_WIDTH);

    #pragma omp parallel for
    for (size_t j = 0; j<SCREEN_HEIGHT; j++) { // actual rendering loop
        for (size_t i = 0; i<SCREEN_WIDTH; i++) {
            double dir_x =  (i + 0.5) -  SCREEN_WIDTH/2.;
            double dir_y = -(j + 0.5) + SCREEN_HEIGHT/2.;    // this flips the image at the same time
            double dir_z = -SCREEN_HEIGHT/(2.*tan(fov/2.));
            framebuffer[j][i] = colorC(cast_ray(cam.pos, rotate(vec3{dir_x, dir_y, dir_z},cam.dir).normalize(), spheres, lights));
            
            #if antialias 
            frame1[j][i] = framebuffer[j][i];
            #endif
        }
    }
    #pragma omp parallel for
    for (size_t j = 0; j<SCREEN_HEIGHT; j++) { // actual rendering loop
        for (size_t i = 0; i<SCREEN_WIDTH; i++) {
            double dir_x =  (i + 0.5) -  SCREEN_WIDTH/2.;
            double dir_y = -(j + 0.5) + SCREEN_HEIGHT/2.;    // this flips the image at the same time
            double dir_z = -SCREEN_HEIGHT/(2.*tan(fov/2.));
            if(checkT(frame1[j][i],frame1[j+1][i+1],threshold[0])|| \
                checkT(frame1[j][i],frame1[j-1][i+1],threshold[0])|| \
                checkT(frame1[j][i],frame1[j-1][i-1],threshold[0])|| \
                checkT(frame1[j][i],frame1[j+1][i-1],threshold[0])|| \
                checkT(frame1[j][i],frame1[j][i+1],threshold[1])|| \
                checkT(frame1[j][i],frame1[j][i-1],threshold[1])|| \
                checkT(frame1[j][i],frame1[j-1][i],threshold[1])|| \
                checkT(frame1[j][i],frame1[j+1][i],threshold[1])){
                //color newC(framebuffer [j][i].r/512.0,framebuffer [j][i].g/512.0,framebuffer [j][i].b/512.0);
                color newC(0,0,0);
                // corners
                newC += cast_ray(cam.pos, rotate(vec3{dir_x+0.25, dir_y+0.25, dir_z},cam.dir).normalize(), spheres, lights)*alias[0];
                newC += cast_ray(cam.pos, rotate(vec3{dir_x-0.25, dir_y+0.25, dir_z},cam.dir).normalize(), spheres, lights)*alias[0];
                newC += cast_ray(cam.pos, rotate(vec3{dir_x-0.25, dir_y-0.25, dir_z},cam.dir).normalize(), spheres, lights)*alias[0];
                newC += cast_ray(cam.pos, rotate(vec3{dir_x+0.25, dir_y-0.25, dir_z},cam.dir).normalize(), spheres, lights)*alias[0];
                // sides
                newC += cast_ray(cam.pos, rotate(vec3{dir_x, dir_y+0.25, dir_z},cam.dir).normalize(), spheres, lights)*alias[1];
                newC += cast_ray(cam.pos, rotate(vec3{dir_x, dir_y-0.25, dir_z},cam.dir).normalize(), spheres, lights)*alias[1];
                newC += cast_ray(cam.pos, rotate(vec3{dir_x-0.25, dir_y, dir_z},cam.dir).normalize(), spheres, lights)*alias[1];
                newC += cast_ray(cam.pos, rotate(vec3{dir_x+0.25, dir_y, dir_z},cam.dir).normalize(), spheres, lights)*alias[1];
                newC /= 3; // .25*4 + .5*4 + .5
                framebuffer[j][i] = colorC(newC);
            }
        }
    }
}


void signal_hand(int signum) {
   std::cout << "Caught signal " << signum << std::endl;
   sdl_pixels_unlock();
   sdl_close(0);
}
void create_objects(Objects& objects ,Lights& lights){
    const Material      ivory = {1.0, {0.6,  0.3, 0.1, 0.0}, {0.4, 0.4, 0.3},   50.};
    const Material      glass = {1.5, {0.0,  0.5, 0.1, 0.8}, {0.6, 0.7, 0.8},  125.};
    const Material red_rubber = {1.0, {0.9,  0.1, 0.0, 0.0}, {0.3, 0.1, 0.1},   10.};
    const Material     mirror = {1.0, {0.0, 10.0, 0.8, 0.0}, {5.0, 1.0, 1.0}, 1425.};
    const Material       wood = {1.0, {1.2,  0.1, 0.0, 0.0}, {0.2, 0.1, 0.02},    1.};

    objects ={{
        Sphere{vec3{-3,    0,   -16}, 2,      ivory},
        Sphere{vec3{-1.0, -1.5, -12}, 2,      glass},
        Sphere{vec3{ 1.5, -0.5, -18}, 3, red_rubber},
        Sphere{vec3{ 7,    5,   -18}, 4,     mirror},
        Sphere{vec3{-3,    10,   -17}, 2,      wood}
    },{
        Triangle{vec3{-3,    0,   -16},vec3{-3,    10,   -17},vec3{ 7,    5,   -18},red_rubber}
    }
    };

    lights = {{
        {{-20, 20,  20}, {1.5,1,1}},
        {{ 30, 50, -25}, {1,1.8,1}},
        {{ 30, 20,  30}, {1,1,1.7}}
    }
    };

}
int main() {
    int winsizeX, winsizeY;
    signal(SIGINT, signal_hand);
	if( !sdl_init() )
	{
		printf( "Failed to initialize!\n" );
        exit(-1);
	}

    Cam cam = {
        {1,1,0},
        {1,1,0},
        M_PI_2
    };
    Objects objects;
    Lights lights;
    create_objects(objects,lights);
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
                xM += event.motion.xrel*MOUSE_SENSITIVITY;
                yM += event.motion.yrel*MOUSE_SENSITIVITY;
                SDL_GetWindowSize(sdl_getwindow(), &winsizeX,&winsizeY);
                cam.dir = vec3{-(yM*M_PI/winsizeX)*1.,-(xM*M_PI/winsizeY)*1,0};
            }
        }
        if(keystates[SDL_SCANCODE_ESCAPE])
            signal_hand(0);
        if(keystates[SDL_SCANCODE_W])
            cam.pos -= rotate(vec3{0,0,MOVEMENT_SPEED},vec3{-(yM*M_PI/winsizeX)*1.,-(xM*M_PI/winsizeY)*1.,0});
        if(keystates[SDL_SCANCODE_S])
            cam.pos += rotate(vec3{0,0,MOVEMENT_SPEED},vec3{-(yM*M_PI/winsizeX)*1.,-(xM*M_PI/winsizeY)*1.,0});
        if(keystates[SDL_SCANCODE_A])
            cam.pos -= rotate(vec3{MOVEMENT_SPEED,0,0},vec3{-(yM*M_PI/winsizeX)*1.,-(xM*M_PI/winsizeY)*1.,0});
        if(keystates[SDL_SCANCODE_D])
            cam.pos += rotate(vec3{MOVEMENT_SPEED,0,0},vec3{-(yM*M_PI/winsizeX)*1.,-(xM*M_PI/winsizeY)*1.,0});
        if(keystates[SDL_SCANCODE_SPACE])
            cam.pos.y += MOVEMENT_SPEED;
        if(keystates[SDL_SCANCODE_LSHIFT])
            cam.pos.y -= MOVEMENT_SPEED;
        render(objects, lights,cam);
        sdl_frame();
    }
    sdl_free();
    return -2; // We shouldn't get here
}