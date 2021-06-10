#include "common.h"
#include "geometry.h"
#include "objects.h"
#ifndef __RAYTRACE_H__
#define __RAYTRACE_H__
bool ray_sphere_intersect(const vec3 &orig, const vec3 &dir, const Sphere &s, double &t0);
vec3 reflect(const vec3 &I, const vec3 &N);
vec3 refract(const vec3 &I, const vec3 &N, const double eta_t, const double eta_i=1.f);
bool scene_intersect(const vec3 &orig, const vec3 &dir, const std::vector<Sphere> &spheres, vec3 &hit, vec3 &N, Material &material);
vec3 cast_ray(const vec3 &orig, const vec3 &dir, const std::vector<Sphere> &spheres, const std::vector<Light> &lights, size_t depth=0);
#endif // __RAYTRACE_H__