
#include "common.h"
#include "vector.h"
#ifndef __OBJECTS_H__
#define __OBJECTS_H__

struct Light {
    vec3 position;
    color intensity; // RGB
};
struct SpotLight{
    vec3 position;
    vec3 dir;
    double fov;
    color intensity; // RGB
};
struct Material {
    double refractive_index = 1;
    vec4 albedo = {1,0,0,0};
    color diffuse_color = {0,0,0};
    double specular_exponent = 0;
};
struct Sphere {
    vec3 center;
    double radius;
    Material material;
};
struct Triangle {
    vec3 a,b,c;
    Material material;
};
struct Cam {
    vec3 pos;
    vec3 dir;
    double fov;
};

struct Ray {
    vec3 o;
    vec3 d;
};


struct Lights{
    std::vector<Light> light;
    std::vector<SpotLight>  spotLight;
};
struct Objects{
    std::vector<Sphere>  sphere;
    std::vector<Triangle>  triangle;
};
#endif // __OBJECTS_H__