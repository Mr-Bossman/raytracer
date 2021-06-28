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
typedef struct futhark_opaque_albedo* Falbedo;
typedef struct futhark_opaque_material* Fmaterial;
typedef struct futhark_context* Fcontext;
typedef struct futhark_context_config* Fconfig;
typedef struct futhark_context_config* Fconfig;
typedef struct futhark_opaque_arr_sphere_1d* FsphereArr;
typedef struct futhark_u32_2d* Fu32_2;
typedef struct futhark_f64_1d* FdoubleA;
Fvec3 Fvec3C(Fcontext ctx, vec3 pos);
Fray FrayC(Fcontext ctx, Ray ray);
Fsphere FsphereC(Fcontext ctx, Sphere s);
Fcam FcamC(Fcontext ctx, Cam c);
Falbedo FalbedoC(Fcontext ctx, vec4 al);
Fvec3 FcolorV3C(Fcontext ctx, color C);
Fmaterial FmaterialC(Fcontext ctx, Material al);
FsphereArr FsphereArrC(Fcontext ctx, std::vector<Sphere> s);
#endif