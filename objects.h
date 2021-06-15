
#include "common.h"
#include "geometry.h"
#include "vector.h"
#ifndef __OBJECTS_H__
#define __OBJECTS_H__

struct Light {
    vec3 position;
    color intensity; // RGB
};

struct Material {
    float refractive_index = 1;
    vec4 albedo = {1,0,0,0};
    color diffuse_color = {0,0,0};
    float specular_exponent = 0;
};

struct Sphere {
    vec3 center;
    float radius;
    Material material;
};
struct Cam {
    vec3 pos;
    vec3 dir;
    float fov;
};
#endif // __OBJECTS_H__