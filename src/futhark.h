#ifndef __FUTHARK__H__
#define __FUTHARK__H__
#include "vector.h"
#include "objects.h"
#include "common.h"
#include "../bin/ray.h"
typedef struct futhark_opaque_state* Fstate;
typedef struct futhark_opaque_vec3* Fvec3;
typedef struct futhark_opaque_ray* Fray;
typedef struct futhark_opaque_cam* Fcam;
typedef struct futhark_opaque_sphere* Fsphere;
typedef struct futhark_opaque_triangle* Ftriangle;
typedef struct futhark_opaque_albedo* Falbedo;
typedef struct futhark_opaque_material* Fmaterial;
typedef struct futhark_context* Fcontext;
typedef struct futhark_context_config* Fconfig;
typedef struct futhark_context_config* Fconfig;
typedef struct futhark_opaque_arr_triangle_1d* FtriangleArr;
typedef struct futhark_opaque_arr_sphere_1d* FsphereArr;
typedef struct futhark_opaque_arr_light_1d* FlightArr;
typedef struct futhark_opaque_arr_vec3_1d* Fvec3Arr;
typedef struct futhark_u32_2d* Fu32_2;
typedef struct futhark_f64_1d* FdoubleA;
Fvec3 Fvec3C(Fcontext ctx, vec3 pos);
Fray FrayC(Fcontext ctx, Ray ray);
Fsphere FsphereC(Fcontext ctx, Sphere s);
Fcam FcamC(Fcontext ctx, Cam c);
Falbedo FalbedoC(Fcontext ctx, vec4 al);
Fvec3 FcolorV3C(Fcontext ctx, color C);
Fmaterial FmaterialC(Fcontext ctx, Material al);
FtriangleArr FtriangleArrC(Fcontext ctx, std::vector<Triangle> l);
FsphereArr FsphereArrC(Fcontext ctx, std::vector<Sphere> s);
FlightArr FlightArrC(Fcontext ctx, std::vector<Light> l);
#endif